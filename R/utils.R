.get_sf_field <- function(type) { 
    search <- "size_factor"
    if (!is.null(type)) { 
        if (length(type)!=1L) { 
            stop("'type' must be a character vector of length 1") 
        }
        search <- paste0(search, "_", type) 
    }
    return(search)
}

.spike_field <- "is_spike"

.get_spike_field <- function(type, check=TRUE) {
    if (check && length(type)!=1L) { 
        stop("'type' must be a character vector of length 1") 
    }
    paste0(.spike_field, "_", type) 
}

.convert_subset <- function(subset, .length, .names) {
    output <- logical(.length)
    names(output) <- .names
    output[subset] <- TRUE
    return(unname(output))
}

