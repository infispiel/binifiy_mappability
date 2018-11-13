# Binify Mappability Data

A script for the sole purpose of taking a UMAP multi-read mappability file & converting it into a 5kB-binned bed file for later use/processing.

## UMAP

The files this script was intended to be used on are obtained here: https://bismap.hoffmanlab.org/

Please note that as of the day of writing this README (11/13/18), the UMAP multi-read file for 50bp reads does not contain the Y chromosome. For the work around used to generate the grch38 binned mappability provided with version 2.0 of the Broad Epi-Correlation pipeline, please see the **Y-Chromosome Workaround** section below.

## How-To

As of version 0.1, the way to run the above code is as follows:

0. Compile binifyBedgraph.cpp using g++
1. Split the bedgraph file into separate chromosomes; i.e. `grep chr1 [bedgraphfile] > bedgraph_chr1.bedGraph`
2. Run the compiled binifyBedgraph on each separated bedgraph file; i.e. `binifyBedgraph bedgraph_chr1.bedGraph > binned_chr1.bed`
3. Re-prefix all files with the appropriate chromosome, i.e. `sed -i -e 's/^/chr1<tab>/' binned_chr1.bed` (ensure that `<tab>` is a true tab character, such as with the key combination "`CTRL+V, <tab>`" in BASH.)
4. Concatenate all files together; i.e. `cat binned_chr*.bed > binned_whole_genome.bed`

A hopefully more straight-forward method will be implemented with the 1.0 release.

This script is entirely assembly-neutral, as it assumes you will add missing tailing bins yourself and will not attempted to pad with 5kb 0 bins until it reaches some certain chromosome size.


## Y-Chromosome Workaround

When this pipline was run to generate the grch38 mappability bed file for the Epi-Correlation pipeline, the available UMAP BedGraph files did not contain any values for the Y chromosome. Despite this, the UCSC browser session link provided on that very same page had values for Y chromosome locations. Therefore, in order to create a complete grch38 track, the .bed file from the provided UCSC session was downloaded, converted into a BedGraph, and _then_ run through this pipeline.

If you are obtaining no values for the Y chromosome, I would strongly suggest you do the same. For example, when creating the separated bedgraph files in step 1 of the How-To, if your bedgraph_chrY.bedGraph file is 0B, that means you are missing the Y chromosome.
