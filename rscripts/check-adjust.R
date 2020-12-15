rm(list = ls())

raw.pval <- read.table("/home/haoliang.xue/Documents/development/pvalueAdjust/test/raw-pvalue.tsv", header = F)
adj.pval <- read.table("/home/haoliang.xue/Documents/development/pvalueAdjust/test/adjusted-pvalue.tsv", header = F)

p.adj <- p.adjust(raw.pval$V2, method = "BH")
p.adj <- p.adj[order(p.adj, decreasing = F)]
max(abs(p.adj - adj.pval$V2))
