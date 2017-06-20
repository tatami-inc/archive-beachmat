setupHDF5Array <- function(dims, storage.mode, chunk, compress)
# This is a convenience wrapper that, if given the matrix dimensions,
# runs a series of R functions and returns the result to C++.
# The idea is to avoid multiple environment calls from C++.
{
    fname <- getHDF5DumpFile(for.use=TRUE)
    dname <- getHDF5DumpName(for.use=TRUE)
    if (any(chunk==0L)) {
        chunk <- getHDF5DumpChunkDim(dims, storage.mode)
    }
    if (compress < 0L) { 
        compress <- getHDF5DumpCompressionLevel()
    }
    appendDatasetCreationToHDF5DumpLog(fname, dname, dims, storage.mode, chunk, compress)
    return(list(fname=fname, dname=dname, chunk=chunk, compress=compress))
}
