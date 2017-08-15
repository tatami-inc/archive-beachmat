\name{pkgconfig}
\alias{pkgconfig}
\title{Compiler configuration arguments for use of beachmat}

\description{
This function returns values for \code{PKG_LIBS} and \code{PKG_CPPFLAGS} variables for use in Makevars files. 
See \code{vignette("beachmat", "beachmat")} for details. 
Only \code{PKG_LIBS} should be needed in most cases.
The environment variable \code{RBEACHMAT_RPATH} can be used to over-ride the inferred location of the installed package.
}

\usage{
pkgconfig(opt = c("PKG_LIBS", "PKG_CPPFLAGS"))
}

\arguments{
\item{opt}{A string specifying the compilation flags to print.}
}

\value{
Returns \code{NULL} and prints the corresponding value to stdout.
}

\author{Aaron Lun}

\examples{
pkgconfig("PKG_LIBS")
pkgconfig("PKG_CPPFLAGS")
}

\keyword{ manip }

