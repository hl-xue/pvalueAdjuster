# pvalueAdjuster

Adjust large p-value list with [Benjamini–Hochberg procedure](https://en.wikipedia.org/wiki/False_discovery_rate).

To compile, clone the repository and ```make```.

```text
Usage: pvalueAdjuster -out-path ADJUSTED_PVAL_PATH RAW_PVAL_PATH
```

Note:

1. The input file contains a column of feature name and a column of raw p-values WITHOUT the header line
2. If input raw p-value is NA, the NA is output for the adjusted p-value
