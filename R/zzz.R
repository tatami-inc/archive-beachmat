pkgconfig <- function(opt = c("PKG_LIBS", "PKG_CPPFLAGS"))
{
    path <- system.file("libs", package="beachmat", mustWork=TRUE)
    if (nzchar(.Platform$r_arch)) {
        arch <- sprintf("/%s", .Platform$r_arch)
    } else {
        arch <- ""
    }
    patharch <- paste0(path, arch)
    extralib <- ifelse(use.hdf5, "-lhdf5_cpp", "")

    result <- switch(match.arg(opt), PKG_CPPFLAGS={
        sprintf('-I"%s"', system.file("include", package="beachmat"))
    }, PKG_LIBS={
        switch(Sys.info()['sysname'], Linux={
            sprintf('-L%s -Wl,-rpath,%s -lbeach %s -pthread', patharch, patharch, extralib)
        }, Darwin={
            sprintf('%s/libbeach.a %s -pthread', patharch, extralib)
        }, Windows={
            sprintf('-L"%s" -lbeach %s -pthread -lws2_32', patharch, extralib)
        }
    )})

    cat(result)
}
