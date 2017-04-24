# Checks for proper construction and get/setting of the slots of a SingleCellExperiment.
# library(beachmat); library(testthat)

set.seed(1000)
ncells <- 100

# Adding assays.
v <- matrix(rnorm(20000), ncol=ncells)
sce <- SingleCellExperiment(assay=v)
expect_equivalent(assay(sce), v)

u <- matrix(rpois(20000, 5), ncol=ncells)
sce <- SingleCellExperiment(assay=list(counts=u, exprs=v))
expect_equivalent(assay(sce, "counts"), u)
expect_equivalent(assay(sce, "exprs"), v)

w <- matrix(runif(20000), ncol=ncells)
assay(sce, "exprs") <- w
expect_equivalent(assay(sce, "exprs"), w)

# Adding metadata.
rd <- DataFrame(stuff=runif(nrow(v)))
cd <- DataFrame(whee=runif(ncells))
sce <- SingleCellExperiment(u, rowData=rd, colData=cd)
expect_equal(rd, rowData(sce))
expect_equal(cd, colData(sce))

cextra <- rnorm(ncells)
sce$blah <- cextra
expect_equal(cextra, colData(sce)$blah)
rextra <- rnorm(nrow(v))
rowData(sce)$blah <- rextra
expect_equal(rextra, rowData(sce)$blah)

sce <- SingleCellExperiment(u, metadata=list(yay=1))
expect_equal(metadata(sce)$yay, 1)
metadata(sce)$yay <- "stuff"
expect_identical(metadata(sce)$yay, "stuff")

# Adding reduced dimensions.
pca <- matrix(runif(ncells*5), ncells)
tsne <- matrix(rnorm(ncells*2), ncells)
combined <- SimpleList(PCA=pca, tSNE=tsne)
sce <- SingleCellExperiment(u, reducedDims=combined)

expect_equivalent(pca, reducedDim(sce, "PCA"))
expect_equivalent(tsne, reducedDim(sce, "tSNE"))
expect_equivalent(reducedDims(sce), combined)

dm <- matrix(runif(ncells*2), ncells)
reducedDim(sce, "DM") <- dm
expect_equivalent(dm, reducedDim(sce, "DM"))
reducedDim(sce, "DM") <- NULL 
expect_equivalent(reducedDims(sce), combined)
reducedDims(sce) <- SimpleList(DM=dm)
expect_equivalent(SimpleList(DM=dm), reducedDims(sce))

expect_error(reducedDims(sce) <- SimpleList(dm), "'reducedDims' must be a named list", fixed=TRUE)
expect_error(reducedDim(sce, "DM") <- dm[1:10,], "'nrow' of each element of 'reducedDims' is not equal to 'ncol(object)'", fixed=TRUE)
expect_error(reducedDim(sce, "DM") <- "huh", "'nrow' of each element of 'reducedDims' is not equal to 'ncol(object)'", fixed=TRUE)

# Checking internals.
sce <- SingleCellExperiment(assay=u)
expect_identical(nrow(beachmat:::int_elementMetadata(sce)), nrow(sce))
expect_identical(nrow(beachmat:::int_colData(sce)), ncol(sce))
expect_identical(length(beachmat:::int_metadata(sce)), 1L)

beachmat:::int_elementMetadata(sce)$whee <- rextra
expect_equal(rextra, beachmat:::int_elementMetadata(sce)$whee)
beachmat:::int_elementMetadata(sce) <- DataFrame(1:5)
expect_error(validObject(sce), "'nrow' of internal 'rowData' not equal to 'nrow(object)'", fixed=TRUE)

beachmat:::int_colData(sce)$stuff <- cextra
expect_equal(cextra, beachmat:::int_colData(sce)$stuff)
beachmat:::int_colData(sce) <- DataFrame(1:5)
expect_error(validObject(sce), "'nrow' of internal 'colData' not equal to 'ncol(object)'", fixed=TRUE)

beachmat:::int_metadata(sce)$urg <- "I was here"
expect_identical(beachmat:::int_metadata(sce)$urg, "I was here")


