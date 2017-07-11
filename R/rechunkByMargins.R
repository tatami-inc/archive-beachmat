rechunkByMargins <- function(x, size=5000, outfile=NULL, outname=NULL, outlevel=NULL, byrow=TRUE) 
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

    # Checking the compression level
    if (is.null(outlevel)) {
        outlevel <- getHDF5DumpCompressionLevel() 
    }
    if (outlevel==0) {
        stop("compression level of 0 implies a contiguous layout")      
    }
    
    # Repacking the file.
    data.type <- type(x)
    chunk.dims <- .Call(cxx_rechunk_matrix, x@seed@file, x@seed@name, data.type, 
                        outfile, outname, outlevel, size, byrow)

    # Generating output. 
    appendDatasetCreationToHDF5DumpLog(outfile, outname, dim(x), data.type, chunk.dims, outlevel)
    HDF5Array(outfile, outname, data.type)
}

