#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

const void PrintHelper()
{
    std::cerr << "[Usage]   pvalueAdjuster -out-path OUT_PATH RAW_PVALUE_PATH" << std::endl
              << std::endl;
}

const void ParseOptions(int argc,
                        char *argv[],
                        std::string &out_path,
                        std::string &raw_pvalue_path)
{
    int i_opt = 1;
    while (i_opt < argc && argv[i_opt][0] == '-')
    {
        std::string arg(argv[i_opt]);
        if (arg == "-h" || arg == "-help")
        {
            PrintHelper();
            exit(EXIT_SUCCESS);
        }
        else if (arg == "-out-path" && i_opt + 1 < argc)
        {
            out_path = argv[++i_opt];
        }
        else
        {
            PrintHelper();
            throw std::invalid_argument("unknown option: " + arg);
        }
        ++i_opt;
    }
    if (i_opt == argc)
    {
        PrintHelper();
        throw std::invalid_argument("raw pvalue path is mandatory");
    }
    raw_pvalue_path = argv[i_opt++];
}

const void ScanPValue(std::vector<std::pair<std::string, double>> &feature_pvalue,
                      const std::string &pvalue_file_path)
{
    std::ifstream pvalue_file(pvalue_file_path);
    if (!pvalue_file.is_open())
    {
        throw std::domain_error("count table " + pvalue_file_path + " was not found");
    }
    boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
    {
        size_t pos = pvalue_file_path.find_last_of(".");
        if (pos != std::string::npos && pvalue_file_path.substr(pos + 1) == "gz")
        {
            inbuf.push(boost::iostreams::gzip_decompressor());
        }
    }
    inbuf.push(pvalue_file);
    std::istream feature_count_instream(&inbuf);

    std::string line, feature_str, val_str;
    std::istringstream conv;
    while (std::getline(feature_count_instream, line))
    {
        conv.str(line);
        conv >> feature_str >> val_str;
        if (val_str == "NA" || val_str == "NAN" || val_str == "na" || val_str == "NaN" || val_str == "nan")
        {
            feature_pvalue.emplace_back(std::make_pair(feature_str, std::nanf("")));
        }
        else
        {
            feature_pvalue.emplace_back(std::make_pair(feature_str, std::stod(val_str)));
        }
        conv.clear();
    }
    pvalue_file.close();
}

void SortPvalue(std::vector<std::pair<std::string, double>> &feature_pvalue)
{
    auto comp = [](std::pair<std::string, double> p1, std::pair<std::string, double> p2) -> bool { return p1.second < p2.second || std::isnan(p2.second); };
    std::sort(feature_pvalue.begin(), feature_pvalue.end(), comp);
}

void PValueAdjustmentBH(std::vector<std::pair<std::string, double>> &feature_pvalue)
{
    long i_feature = feature_pvalue.size() - 1;
    while (std::isnan(feature_pvalue[i_feature].second))
    {
        --i_feature;
    }
    double tot_num = i_feature + 1, adjust_factor;
    --i_feature; // the last pvalue need not to be adjusted (adjust_factor == 1)
    while (i_feature >= 0)
    {
        adjust_factor = tot_num / (i_feature + 1);
        feature_pvalue[i_feature].second *= adjust_factor;
        if (feature_pvalue[i_feature].second > feature_pvalue[i_feature + 1].second)
        {
            feature_pvalue[i_feature].second = feature_pvalue[i_feature + 1].second;
        }
        --i_feature;
    }
}

void PvaluePrint(const std::vector<std::pair<std::string, double>> &feature_pvalue,
                 const std::string &out_path)
{
    std::ofstream out_file(out_path);
    if (!out_file.is_open())
    {
        throw std::domain_error("cannot open file: " + out_path);
    }
    for (const auto &kp : feature_pvalue)
    {
        if (std::isnan(kp.second))
        {
            out_file << kp.first << "\tNA" << std::endl;
        }
        else
        {
            out_file << kp.first << "\t" << kp.second << std::endl;
        }
    }
    out_file.close();
}

int main(int argc, char *argv[])
{
    std::string raw_pvalue_path, out_path;

    ParseOptions(argc, argv, out_path, raw_pvalue_path);

    std::clock_t begin_time = clock(), inter_time;
    std::vector<std::pair<std::string, double>> feature_pvalue;

    ScanPValue(feature_pvalue, raw_pvalue_path);
    std::cerr << "Raw p-value scanning finished, execution time: " << (float)(clock() - begin_time) / CLOCKS_PER_SEC << "s." << std::endl;
    inter_time = clock();

    SortPvalue(feature_pvalue);
    std::cerr << "P-value sorting finished, execution time: " << (float)(clock() - inter_time) / CLOCKS_PER_SEC << "s." << std::endl;
    inter_time = clock();

    PValueAdjustmentBH(feature_pvalue);
    std::cerr << "P-value adjusting finished, execution time: " << (float)(clock() - inter_time) / CLOCKS_PER_SEC << "s." << std::endl;
    inter_time = clock();

    PvaluePrint(feature_pvalue, out_path);
    std::cerr << "Output finished, execution time: " << (float)(clock() - inter_time) / CLOCKS_PER_SEC << "s." << std::endl;
    inter_time = clock();

    std::cerr << "Executing time: " << (float)(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
    return EXIT_SUCCESS;
}
