# Defines the SingleCellExperiment class.

setClass("SingleCellExperiment", 
         slots=c(int_elementMetadata = "DataFrame",
                 int_colData = "DataFrame",
                 int_metadata = "list",
                 reducedDims = "SimpleList"),
         contains = "SummarizedExperiment")

#############################################
# Sets the validity checker.

.sce_validity <- function(object) {
    msg <- NULL

    # Checking dimensions of reduced coordinates.
    rd <- reducedDims(object)
    if (length(names(rd))!=length(rd)) { 
        stop("'reducedDims' must be a named list")
    }        
    if (length(rd)) { 
        if (any(unlist(lapply(rd, .not_reddim_mat, object=object)))) { 
            msg <- c(msg, "'nrow' of each element of 'reducedDims' is not equal to 'ncol(object)'")
        }
    }

    # Checking dimensions of internal objects.
    if (nrow(int_elementMetadata(object))!=nrow(object)) { 
        msg <- c(msg, "'nrow' of internal 'rowData' not equal to 'nrow(object)'")
    }
    if (nrow(int_colData(object))!=ncol(object)) { 
        msg <- c(msg, "'nrow' of internal 'colData' not equal to 'ncol(object)'")
    }
    
    # Checking version.
    v <- objectVersion(object)
    if (!grepl("^0\\.98", v)) {
        msg <- c(msg, "object is out of date, update with 'updateSCE(object)'")
    }
   
    if (length(msg)) { return(msg) }
    return(TRUE)
}

.not_reddim_mat <- function(val, object) {
    return(!is.matrix(val) || nrow(val)!=ncol(object));
}

setValidity2("SingleCellExperiment", .sce_validity)

#############################################
# Sets the show method.

scat <- function(fmt, vals=character(), exdent=2, ...) {
    vals <- ifelse(nzchar(vals), vals, "''")
    lbls <- paste(S4Vectors:::selectSome(vals), collapse=" ")
    txt <- sprintf(fmt, length(vals), lbls)
    cat(strwrap(txt, exdent=exdent, ...), sep="\n")
}

.sce_show <- function(object) {
    callNextMethod()
    scat("reduced(%d): %s\n", names(reducedDims(object)))
    scat("spikes(%d): %s\n", spikeNames(object))
}

setMethod("show", "SingleCellExperiment", .sce_show)

#############################################
# Defines a constructor.

SingleCellExperiment <- function(..., reducedDims=SimpleList()) {
    se <- SummarizedExperiment(...)
    out <- new("SingleCellExperiment", se, reducedDims=SimpleList(), 
               int_elementMetadata=DataFrame(matrix(0, nrow(se), 0)),
               int_colData=DataFrame(matrix(0, ncol(se), 0)),
               int_metadata=list(version=packageVersion("beachmat")))
    reducedDims(out) <- reducedDims
    return(out)
}

#############################################
# Define subsetting methods.

setMethod("[", c("SingleCellExperiment", "ANY", "ANY"), function(x, i, j, ..., drop=TRUE) {
    if (!missing(i)) { 
        int_elementMetadata(x) <- int_elementMetadata(x)[i,] 
    }

    if (!missing(j)) { 
        int_colData(x) <- int_colData(x)[j,]
        rd <- reducedDims(x)
        for (mode in seq_along(rd)) { rd[[mode]] <- rd[[mode]][j,,drop=FALSE] }
        int_reducedDims(x) <- rd
    }

    callNextMethod()
})

.standardize_DataFrames <- function(...) {
    all.d <- list(...)
    all.fields <- Reduce(union, lapply(all.d, colnames))

    for (d in seq_along(all.d)) { 
        cur.d <- all.d[[d]]
        missing.fields <- setdiff(all.fields, colnames(cur.d))
        for (val in missing.fields) { cur.d[[val]] <- NA }
        all.d[[d]] <- cur.d[,all.fields,drop=FALSE]
    }

    return(all.d)
}

.standardize_reducedDims <- function(...) {
    args <- list(...)
    all.ncells <- lapply(args, ncol)
    all.rd <- lapply(args, reducedDims)
    all.modes <- Reduce(union, lapply(all.rd, names))

    for (m in all.modes) {
        all.dims <- integer(length(all.rd))
        for (d in seq_along(all.rd)) {
            current <- all.rd[[d]][[m]]
            if (!is.null(current)) { 
                all.dims[d] <- ncol(current)
            } else {
                all.dims[d] <- NA_integer_
            }
        }
        
        # Checking consistency of dimensions between objects.
        ok <- !is.na(all.dims)
        udim <- unique(all.dims[ok])
        if (length(udim)!=1) {
            stop(sprintf("dimensions of '%s' are not consistent between objects"), m)
        }
        
        # Filling in dimensions, for those who are missing them.
        for (d in which(!ok)) { 
            all.rd[[d]][[m]] <- matrix(NA_real_, all.ncells[[d]], udim) 
        }
    }
   
    # Standardizing the order.
    for (d in seq_along(all.rd)) {
        all.rd[[d]] <- all.rd[[d]][all.modes]
    }
    return(all.rd)
}

setMethod("[<-", c("SingleCellExperiment", "ANY", "ANY", "SingleCellExperiment"), function(x, i, j, ..., value) {
    if (!missing(i)) { 
        sout <- .standardize_DataFrames(first=int_elementMetadata(x), last=int_elementMetadata(value))
        sout$first[i,] <- sout$last
        int_elementMetadata(x) <- sout$first
    }

    if (!missing(j)) {
        sout <- .standardize_DataFrames(first=int_colData(x), last=int_colData(value))
        sout$first[j,] <- sout$last
        int_colData(x) <- sout$first

        rdout <- .standardize_reducedDims(first=x, last=value)
        rd <- rdout$first
        rdv <- rdout$last
        for (mode in seq_along(rd)) { 
            rd[[mode]][j,] <- rdv[[mode]] 
        }
        int_reducedDims(x) <- rd
    }

    callNextMethod()
})

setMethod("subset", "SingleCellExperiment", function(x, i, j) {
    x[i, j]
})

#############################################
# Defining the combining methods.

setMethod("cbind", "SingleCellExperiment", function(..., deparse.level=1) {
    args <- unname(list(...))
    base <- do.call(cbind, lapply(args, function(x) { as(x, "SummarizedExperiment") }))

    all.col.data <- lapply(args, int_colData)
    sout <- do.call(.standardize_DataFrames, all.col.data)
    new.col.data <- do.call(rbind, sout)    

    all.rd <- do.call(.standardize_reducedDims, args)
    new.rd <- SimpleList(do.call(mapply, c(all.rd, FUN=rbind, SIMPLIFY=FALSE)))
    
    ans <- args[[1]]
    new("SingleCellExperiment", base, int_colData=new.col.data, int_elementMetadata=int_elementMetadata(ans),
        int_metadata=int_metadata(ans), reducedDims=new.rd) 
})

setMethod("rbind", "SingleCellExperiment", function(..., deparse.level=1) {
    args <- unname(list(...))
    base <- do.call(rbind, lapply(args, function(x) { as(x, "SummarizedExperiment") }))

    all.row.data <- lapply(args, int_elementMetadata)
    sout <- do.call(.standardize_DataFrames, all.row.data)
    new.row.data <- do.call(rbind, sout)    

    ans <- args[[1]]
    new("SingleCellExperiment", base, int_colData=int_colData(ans), int_elementMetadata=new.row.data,
        int_metadata=int_metadata(ans), reducedDims=reducedDims(ans))
})

setMethod("c", "SingleCellExperiment", function(x, ..., recursive = FALSE) {
    rbind(x, ...)
})

