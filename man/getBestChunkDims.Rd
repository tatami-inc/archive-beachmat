\alias{getBestChunkDims}
\name{getBestChunkDims}

\title{Get best chunk dimensions}
\description{Computes the optimal chunk dimensions for consecutive row/column access from a HDF5Matrix.}

\usage{
getBestChunkDims(dims)
}

\arguments{
\item{dims}{An integer vector of length 2 containing the dimensions of a HDF5Matrix object.}
}

\details{
Consider a HDF5Matrix where access to consecutive rows or columns is requested.
The optimal chunk dimensions ensure that the number of disk reads required is the same as that of a file layout with pure row or column chunks.
This exploits the HDF5 chunk cache to store data from neighbouring rows/columns, avoiding the need to reread or rewrite the entire chunk for the next row/column.
Obviously, this is not relevant to situations involving random row or column access.
}

\value{
An integer vector of length 2, containing the dimensions of each chunk in the HDF5 file.
}

\author{
Aaron Lun
}

\examples{
getBestChunkDims(c(10340, 234))
getBestChunkDims(c(13400, 2068))
}
