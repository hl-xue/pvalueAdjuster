rm(list = ls())

out.path <- "/home/haoliang.xue/Documents/development/pvalueAdjust/test/raw-pvalue.tsv"

set.seed(91400)
nb <- 100000

kmer_pval <- data.frame("kmer" = paste0("kmer_", 1 : nb),
                        "pval" = runif(n = nb, min = 0, max = 1))
write.table(kmer_pval, out.path, row.names = F, col.names = F, quote = F, sep = "\t")
