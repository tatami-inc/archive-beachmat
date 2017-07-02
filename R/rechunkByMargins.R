rechunkByMargins <- function(x, guide=5000, outfile=NULL, outname=NULL, byrow=TRUE) 
# Creates a new HDF5Matrix with a pure-row or pure-column chunking scheme.
# 
# written by Aaron Lun
# created 2 July 2017
{
    if (!is(x, "HDF5Matrix")) { 
        stop("'x' should be a HDF5Matrix object")
    }

    # Setting names, if not present.
    if (is.null(outfile)) { 
        outfile <- getHDF5DumpFile(for.use=TRUE)        
    } else if (!h5createFile(outfile)) {
        stop("failed to create output HDF5 file")
    }
    if (is.null(outname)) { 
        outname <- getHDF5DumpName(for.use=TRUE)        
    }

    # Repacking the file.
    data.type <- type(x)
    chunk.dims <- .Call(cxx_rechunk_matrix, x@seed@file, x@seed@name, data.type, outfile, outname, guide, byrow)

    # Generating output. 
    appendDatasetCreationToHDF5DumpLog(outfile, outname, dim(x), data.type, chunk.dims, getHDF5DumpCompressionLevel())
    HDF5Array(outfile, outname, data.type)
}

