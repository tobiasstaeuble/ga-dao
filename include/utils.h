#ifndef UTILS_H
#define UTILS_H

class Utils
{
public:

	static void printAndSaveBestGenomes(mogal::Genome * bestGenome, int generation);
	static void saveDoseMatrix(mogal::Genome* bestGenome, int generation);
	bool loadData();
	bool calculateRelativeWeights();
	bool defineObjectives();

private:

};

#endif