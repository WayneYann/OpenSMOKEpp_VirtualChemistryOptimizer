/*-----------------------------------------------------------------------*\
|    ___                   ____  __  __  ___  _  _______                  |
|   / _ \ _ __   ___ _ __ / ___||  \/  |/ _ \| |/ / ____| _     _         |
|  | | | | '_ \ / _ \ '_ \\___ \| |\/| | | | | ' /|  _| _| |_ _| |_       |
|  | |_| | |_) |  __/ | | |___) | |  | | |_| | . \| |__|_   _|_   _|      |
|   \___/| .__/ \___|_| |_|____/|_|  |_|\___/|_|\_\_____||_|   |_|        |
|        |_|                                                              |
|                                                                         |
|   Author: Alberto Cuoci <alberto.cuoci@polimi.it>                       |
|   CRECK Modeling Group <http://creckmodeling.chem.polimi.it>            |
|   Department of Chemistry, Materials and Chemical Engineering           |
|   Politecnico di Milano                                                 |
|   P.zza Leonardo da Vinci 32, 20133 Milano                              |
|                                                                         |
|-------------------------------------------------------------------------|
|                                                                         |
|   This file is part of OpenSMOKE++ framework.                           |
|                                                                         |
|	License                                                               |
|                                                                         |
|   Copyright(C) 2018  Alberto Cuoci                                      |
|   OpenSMOKE++ is free software: you can redistribute it and/or modify   |
|   it under the terms of the GNU General Public License as published by  |
|   the Free Software Foundation, either version 3 of the License, or     |
|   (at your option) any later version.                                   |
|                                                                         |
|   OpenSMOKE++ is distributed in the hope that it will be useful,        |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of        |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
|   GNU General Public License for more details.                          |
|                                                                         |
|   You should have received a copy of the GNU General Public License     |
|   along with OpenSMOKE++. If not, see <http://www.gnu.org/licenses/>.   |
|                                                                         |
\*-----------------------------------------------------------------------*/

#ifndef OpenSMOKE_OptimizationRules_PlugFlowReactor_H
#define OpenSMOKE_OptimizationRules_PlugFlowReactor_H

namespace OpenSMOKE
{
	class Grammar_OptimizationRules_PlugFlowReactor : public OpenSMOKE::OpenSMOKE_DictionaryGrammar
	{
	protected:

		virtual void DefineRules()
		{
			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Species",
				OpenSMOKE::SINGLE_STRING,
				"Name of the species to be optimized",
				true));

			AddKeyWord(OpenSMOKE::OpenSMOKE_DictionaryKeyWord("@Profile",
				OpenSMOKE::SINGLE_DICTIONARY,
				"Name of the dictionary containing the profile",
				true));
		}
	};

	class OptimizationRules_PlugFlowReactor
	{
	public:

		void SetupFromDictionary(OpenSMOKE::OpenSMOKE_Dictionary& dictionary, OpenSMOKE::OpenSMOKE_DictionaryManager& dictionaries);

		unsigned int np() const { return np_; }
		double x(const unsigned int i) const { return x_(i); }
		double y(const unsigned int i) const { return y_(i); }
		double mean_y() const { return mean_y_; }
		double max_y() const { return y_.maxCoeff(); }
		std::string species_name() const { return species_name_; }

	private:

		unsigned int np_;
		Eigen::VectorXd x_;
		Eigen::VectorXd y_;
		double mean_y_;
		std::string species_name_;
	};

	void OptimizationRules_PlugFlowReactor::SetupFromDictionary(OpenSMOKE::OpenSMOKE_Dictionary& dictionary, OpenSMOKE::OpenSMOKE_DictionaryManager& dictionaries)
	{
		Grammar_OptimizationRules_PlugFlowReactor grammar;
		dictionary.SetGrammar(grammar);

		if (dictionary.CheckOption("@Species") == true)
			dictionary.ReadString("@Species", species_name_);

		if (dictionary.CheckOption("@Profile") == true)
		{
			std::string name_of_subdictionary;
			dictionary.ReadDictionary("@Profile", name_of_subdictionary);

			OpenSMOKE::OpenSMOKEVectorDouble x, y;
			std::string x_variable, y_variable;
			GetXYProfileFromDictionary(dictionaries(name_of_subdictionary), x, y, x_variable, y_variable);

			np_ = x.Size();
			x_.resize(np_);
			y_.resize(np_);
			mean_y_ = 0.;
			for (unsigned int i = 0; i < np_; i++)
			{
				x_(i) = x[i + 1];
				y_(i) = y[i + 1];
				mean_y_ += y_(i);
			}
			mean_y_ /= static_cast<double>(np_);
		}
	}
}

#endif // OpenSMOKE_OptimizationRules_PlugFlowReactor_H