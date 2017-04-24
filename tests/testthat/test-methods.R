# Checks for proper functioning of the methods.
# library(beachmat); library(testthat)

set.seed(1000)
ncells <- 100
v <- matrix(rnorm(20000), ncol=ncells)
sce <- SingleCellExperiment(assay=v)

# Adding spike-ins.
is.spike1 <- rbinom(nrow(v), 1, 0.2)==1
isSpike(sce, "ERCC") <- is.spike1
expect_identical(spikeNames(sce), "ERCC")
expect_identical(isSpike(sce, "ERCC"), is.spike1)
expect_identical(isSpike(sce), is.spike1)

is.spike2 <- rbinom(nrow(v), 1, 0.3)==1
isSpike(sce, "SIRV") <- is.spike2
expect_identical(spikeNames(sce), c("ERCC", "SIRV"))
expect_identical(isSpike(sce, "ERCC"), is.spike1) # check still the same.
expect_identical(isSpike(sce, "SIRV"), is.spike2)
expect_identical(isSpike(sce), is.spike1 | is.spike2)

isSpike(sce, "ERCC") <- NULL
expect_identical(spikeNames(sce), "SIRV")
expect_error(isSpike(sce, "ERCC"), "spike-in set 'ERCC' does not exist")
expect_identical(isSpike(sce, "SIRV"), is.spike2)
expect_identical(isSpike(sce), is.spike2)

chosen <- sample(nrow(v), 20) # integer setting
isSpike(sce, "ERCC") <- chosen
expect_identical(which(isSpike(sce, "ERCC")), sort(chosen)) 
expect_identical(spikeNames(sce), c("SIRV", "ERCC")) # flipped

rownames(sce) <- paste0("Gene", seq_len(nrow(v))) # character setting
isSpike(sce, "SIRV") <- rownames(sce)[chosen]
expect_identical(which(isSpike(sce, "SIRV")), sort(chosen))

beachmat:::int_metadata(sce)$spike_names <- c("random")
expect_error(validObject(sce), "no field specifying rows belonging to spike-in set 'random'", fixed=TRUE)

# Adding size factors.
sf1 <- 2^rnorm(ncells)
sizeFactors(sce) <- sf1
expect_identical(sizeFactors(sce), sf1)

sf2 <- 2^rnorm(ncells, sd=2)
sizeFactors(sce, "ERCC") <- sf2
expect_identical(sizeFactors(sce), sf1) # check still the same
expect_identical(sizeFactors(sce, "ERCC"), sf2)

sizeFactors(sce) <- NULL
expect_identical(sizeFactors(sce), NULL)
expect_identical(sizeFactors(sce, "ERCC"), sf2) # check still the same

sizeFactors(sce, "ERCC") <- NULL
expect_identical(sizeFactors(sce, "ERCC"), NULL)

# Checking package version.
expect_identical(objectVersion(sce), packageVersion("beachmat"))

