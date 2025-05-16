#include <iostream>
#include <fstream>

namespace le {

    template <typename T>
    class PersistentData
    {
    private:
        std::string filename;
        std::ofstream outfile;

    public:
        PersistentData(const std::string& file) : filename(file), outfile(file, std::ios::app)
        {
            if (!outfile.is_open())
                std::cerr << "Unable to open file for writing!" << std::endl;
        }

        ~PersistentData()
        {
            if (outfile.is_open())
                outfile.close();
        }

        T read()
        {
            T value;
            std::ifstream infile(filename);
            if (infile.is_open())
            {
                infile >> value;
                infile.close();
                return value;
            }
            else {
                std::cerr << "Unable to open file for reading!" << std::endl;
                return T();  // return default-constructed value
            }
        }

        void update(const T& newValue)
        {
            if (outfile.is_open())
            {
                outfile.seekp(0);             // Go to the beginning of the file
                outfile << newValue;          // Write the new value
                outfile.flush();              // Explicitly flush the buffer
            }
            else {
                std::cerr << "File isn't open for writing!" << std::endl;
            }
        }
    };

} // namespace le