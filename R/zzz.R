pkgconfig <- function(opt = c("PKG_LIBS", "PKG_CPPFLAGS"))
{
    path <- system.file("libs", package="beachmat", mustWork=TRUE)
    if (nzchar(.Platform$r_arch)) {
        arch <- sprintf("/%s", .Platform$r_arch)
    } else {
        arch <- ""
    }
    patharch <- paste0(path, arch)

    result <- switch(match.arg(opt), PKG_CPPFLAGS={
        sprintf('-I"%s"', system.file("include", package="beachmat"))
    }, PKG_LIBS={
        switch(Sys.info()['sysname'], Linux={
            sprintf('-L%s -Wl,-rpath,%s -lbeach -lhdf5_cpp -pthread',
                    patharch, patharch)
        }, Darwin={
            sprintf('%s/libbeach.a -lhdf5_cpp -pthread', patharch)
        }, Windows={
            sprintf('-L"%s" -lbeach -lhdf5_cpp -pthread -lws2_32', patharch)
        }
    )})

    cat(result)
}
