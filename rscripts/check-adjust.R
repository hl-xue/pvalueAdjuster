rm(list = ls())

cmdArgs <- commandArgs(trailingOnly = T)
raw.pval.path <- cmdArgs[1]
out.path <- cmdArgs[2]

# raw.pval.path <- "/home/haoliang.xue/Documents/development/pvalueAdjust/test/raw-pvalue.tsv"
# out.path <- "/home/haoliang.xue/Documents/development/pvalueAdjust/test/adjusted-pvalue.tsv"

pval.tab <- read.table(raw.pval.path, header = F)
names(pval.tab) <- c("feature", "p.raw")
pval.tab$p.adj.byR <- p.adjust(pval.tab$p.raw, method = "BH")

write.table(pval.tab, file = out.path, row.names = F, col.names = T, quote = F, sep = "\t")
