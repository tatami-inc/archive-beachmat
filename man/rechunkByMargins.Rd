\name{rechunkByMargins}
\alias{rechunkByMargins}

\title{Rechunk by margins}
\description{Convert an existing HDF5Matrix into a pure column- or row-based chunk layout.}

\usage{
rechunkByMargins(x, guide=5000, outfile=NULL, outname=NULL, byrow=TRUE) 
}

\arguments{
\item{x}{A HDF5Matrix object.}
\item{guide}{An integer scalar specifying the approximate number of elements in each chunk.}
\item{outfile}{A string containing the name for the output HDF5 file, chosen by \code{\link{getHDF5DumpFile}} if not specified.}
\item{outname}{A string containing the name for the output HDF5 data set, chosen by \code{\link{getHDF5DumpName}} if not specified.}
\item{byrow}{A logical scalar indicating if the output file should be row-chunked (default) or column-chunked.}
}

\details{
Pure column- or row-based chunk layouts are optimal for random column and row access, respectively, from a HDF5 file.
This function can be used to convert a file into a pure row/column layout prior to calling other functions.
In many cases, a small investment in rechunking time is repaid by a reduction in access times in downstream procedures.

The \code{guide} argument is only an approximate indicator as to the size of the output chunks.
To maintain performance, this function will read and write in entire chunks.
As such, the actual output chunk size is chosen to be the smallest multiple of the corresponding input chunk dimension.
}

\value{
A HDF5Matrix object pointing to the HDF5 file containing the data from \code{x} but with the new chunk layout.
}

\author{Aaron Lun}

\examples{
A <- as(matrix(runif(5000), nrow=100, ncol=50), "HDF5Array")
byrow <- rechunkByMargins(A, byrow=TRUE)
bycol <- rechunkByMargins(A, byrow=FALSE)
}
