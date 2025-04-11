#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <random>
#include <string>
#include <fstream>
#include <ctime>

// enumeration for months
enum Months {
    JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
};

// Array which maps month enums to string representations
const std::string MONTH_NAMES[] = {
    "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

// Data of weather
class WeatherData {
    private:
        // 2d array for historical data
        double** historicalData;
        // number of years
        int years;
        // months per year
        int months;
        // average temp of each month
        std::vector<double> monthlyAverages;
        // predicted temp for 2025
        std::vector<double> predictions;

    public:
        // constructor
        WeatherData(int years, int months) : years(years), months(months) {
            historicalData = new double*[years];
            for (int i = 0; i < years; i++)
            {
                historicalData[i] = new double[months];
            }
        }

        // destructor
        ~WeatherData() {
            for (int i = 0; i < years; i++)
            {
                delete[] historicalData[i];
            }
            delete[] historicalData;
        }

        // enter the historical data into the dynamically allocated array
        void loadHistoricalData(const double data[5][12])
        {
            for (int i = 0; i < years; i++)
            {
                for (int j = 0; j < months; j++)
                {
                    historicalData[i][j] = data[i][j];
                }
            }
        }

        // calculate the average temp for each month (historical data)
        void calculationMonthlyAverages()
        {
            for (int j = 0; j < months; j++)
            {
                double total = 0.0;
                for (int i = 0; i < years; i++)
                {
                    total += historicalData[i][j];
                }
                double average = total / years;
                monthlyAverages.push_back(average);
            }
        }

        // 2025 average temp predictions
        void predict2025()
        {
            // normal distribution for the available data
            std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));
            std::normal_distribution<double> fluctuation(0.0, 0.5);
            
            for (int i = 0; i < months; i++)
            {
                double prediction = monthlyAverages[i] + fluctuation(generator);
                predictions.push_back(prediction);
            }
        }

        // display historical monthly data
        void displayHistoricalAverages() const 
        {
            std::cout << "\n===== Historical Monthly Averages for 2020-2024 =====\n";
            std::cout << std::setw(15) << "Month" << std::setw(20) << "Average Temperature (Celsius)\n";
            std::cout << "---------------------------------------------\n";
            for (int i = 0; i < months; i++)
            {
                std::cout << std::setw(15) << MONTH_NAMES[i] << std::setw(20) << std::fixed << std::setprecision(2) << monthlyAverages[i] << std::endl;
            }
        }

        // display predicted monthly data
        void displayPredictions() const
        {
            std::cout << "\n===== Predicted Monthly Averages for 2025 =====\n";
            std::cout << std::setw(15) << "Month" << std::setw(20) << "Predicted Temperature (Celsius)\n";
            std::cout << "---------------------------------------------\n";
            for (int i = 0; i < months; i++)
            {
                std::cout << std::setw(15) << MONTH_NAMES[i] << std::setw(20) << std::fixed << std::setprecision(2) << predictions[i] << std::endl;
            }
        }

        // save predicted temp
        void savePredictionsToFile(const std::string& filename) const
        {
            std::ofstream file(filename);
            if (file.is_open())
            {
                // predictions to file
                file << "Predicted Monthly Averages for 2025:\n";
                file << std::setw(15) << "Month" << std::setw(20) << "Temperature (Celsius)\n";
                file << "---------------------------------------------\n";
                for (int i = 0; i < months; i++)
                {
                    file << std::setw(15) << MONTH_NAMES[i] << std::setw(20) << std::fixed << std::setprecision(2) << predictions[i] << std::endl;
                }
                file << "\nGenerated on: " << std::asctime(std::localtime(nullptr));
                file.close();
                std::cout << "\nPredictions saved to " << filename << "\n"; 
            } else
            {
                std::cerr << "Error. Unable to open the file!\n";
            }
        }
};

int main()
{
    // declare number of years in the dataset
    const int YEARS = 5;
    // declare number of months per year
    const int MONTHS = 12;

    // historical temp data for the years 2020-2024
    double historicalData[YEARS][MONTHS] = {
        {12.5, 13.6, 15.4, 18.8, 22.4, 26.1, 29.5, 29.2, 27.3, 23.1, 18.5, 14.3},
        {12.2, 13.4, 15.1, 18.6, 22.1, 25.8, 29.2, 28.9, 27.0, 22.8, 18.2, 14.0},
        {12.7, 13.8, 15.7, 19.0, 22.7, 26.4, 29.8, 29.5, 27.6, 23.4, 18.8, 14.6},
        {12.4, 13.5, 15.3, 18.7, 22.3, 26.0, 29.3, 29.0, 27.1, 22.9, 18.3, 14.1},
        {12.6, 13.7, 15.5, 18.9, 22.5, 26.2, 29.6, 27.4, 23.2, 18.7, 18.7, 14.5}
    };

    // initiate class
    WeatherData weather(YEARS, MONTHS);

    // load the historical data
    weather.loadHistoricalData(historicalData);

    // calculate the monthly averages
    weather.calculationMonthlyAverages();
    // predict temp for 2025
    weather.predict2025();

    // display the results
    weather.displayHistoricalAverages();
    weather.displayPredictions();

    // save the predictions to a file
    weather.savePredictionsToFile("Predictions2025.txt");

    return 0;
}