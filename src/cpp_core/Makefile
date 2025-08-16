CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -fPIC
TARGET = libportfolio_optimizer.so

SOURCES = PortfolioOptimizer.cpp \
          MatrixOperations.cpp \
          CSVProcessor.cpp \
          RiskCalculator.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	$(CXX) $(CXXFLAGS) test_portfolio.cpp -L. -lportfolio_optimizer -o test_portfolio
	./test_portfolio