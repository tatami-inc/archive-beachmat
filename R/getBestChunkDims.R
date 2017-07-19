getBestChunkDims <- function(dims) 
# Computes the optimal chunk dimensions for a given
# set of input dimensions for a particular matrix.
#
# written by Aaron Lun
# created 19 July 2017    
{
    .Call(cxx_find_chunks, dims)
}
