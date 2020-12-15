rm(list = ls())

cmdArgs <- commandArgs(trailingOnly = T)
raw.pval.path <- cmdArgs[1]
adj.pval.path <- cmdArgs[2]

raw.pval.path <- "/home/haoliang.xue/Documents/development/pvalueAdjust/test/raw-pvalue.tsv"
adj.pval.path <- "/home/haoliang.xue/Documents/development/pvalueAdjust/test/adjusted-pvalue.tsv"

raw.pval <- read.table(raw.pval.path, header = F)
names(raw.pval) <- c("feature", "p.raw")
raw.pval$p.adj.byR <- p.adjust(raw.pval$p.raw, method = "BH")
raw.pval <- na.omit(raw.pval)

adj.pval <- read.table(adj.pval.path, header = F)
names(adj.pval) <- c("feature", "p.adj")
adj.pval <- na.omit(adj.pval)

cmp.pval <- merge(raw.pval[, c("feature", "p.adj.byR")], adj.pval, by = "feature", all = T)
max(abs(cmp.pval$p.adj - cmp.pval$p.adj.byR))
