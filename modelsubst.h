//
// C++ Interface: substmodel
//
// Description: 
//
//
// Author: BUI Quang Minh, Steffen Klaere, Arndt von Haeseler <minh.bui@univie.ac.at>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SUBSTMODEL_H
#define SUBSTMODEL_H

#include <string>
#include "tools.h"
#include "optimization.h"

using namespace std;

/**
Substitution model abstract class

	@author BUI Quang Minh, Steffen Klaere, Arndt von Haeseler <minh.bui@univie.ac.at>
*/
class ModelSubst: public Optimization
{
	friend class ModelFactory;

public:
	/**
		constructor
		@param nstates number of states, e.g. 4 for DNA, 20 for proteins.
	*/
    ModelSubst(int nstates);


	/**
		@return the number of dimensions
	*/
	virtual int getNDim() { return 0; }

	/**
		@return TRUE if model is time-reversible, FALSE otherwise
	*/
	virtual bool isReversible() { return true; };
	
	/**
	 * @return TRUE if this is a site-specific model, FALSE otherwise
	 */
	virtual bool isSiteSpecificModel() { return false; }
	
	/**
		@return the number of rate entries, equal to the number of elements
			in the upper-diagonal of the rate matrix (since model is reversible)
	*/
	virtual int getNumRateEntries() { return num_states*(num_states-1)/2; }

	/**
	 * get the size of transition matrix, default is num_states*num_states.
	 * can be changed for e.g. site-specific model
	 */
	virtual int getTransMatrixSize() { return num_states * num_states; }

	/**
		compute the transition probability matrix. One should override this function when defining new model.
		The default is the Juke-Cantor model, valid for all kind of data (DNA, AA, Codon, etc)
		@param time time between two events
		@param trans_matrix (OUT) the transition matrix between all pairs of states. 
			Assume trans_matrix has size of num_states * num_states.
	*/
	virtual void computeTransMatrix(double time, double *trans_matrix);

	/**
	 * wrapper for computing transition matrix times state frequency vector
	 * @param time time between two events
	 * @param trans_matrix (OUT) the transition matrix between all pairs of states.
	 * 	Assume trans_matrix has size of num_states * num_states.
	 */
	virtual void computeTransMatrixFreq(double time, double *trans_matrix);

	/**
		compute the transition probability between two states. 
		One should override this function when defining new model.
		The default is the Juke-Cantor model, valid for all kind of data (DNA, AA, Codon, etc)
		@param time time between two events
		@param state1 first state
		@param state2 second state
	*/
	virtual double computeTrans(double time, int state1, int state2);

	/**
		compute the transition probability between two states at a specific model ID, useful for partition model 
		One should override this function when defining new model.
		The default is the Juke-Cantor model, valid for all kind of data (DNA, AA, Codon, etc)
		@param time time between two events
		@param model_id model ID
		@param state1 first state
		@param state2 second state
	*/
	virtual double computeTrans(double time, int model_id, int state1, int state2);

	/**
		compute the transition probability and its 1st and 2nd derivatives between two states. 
		One should override this function when defining new model.
		The default is the Juke-Cantor model, valid for all kind of data (DNA, AA, Codon, etc)
		@param time time between two events
		@param state1 first state
		@param state2 second state
		@param derv1 (OUT) 1st derivative
		@param derv2 (OUT) 2nd derivative
	*/
	virtual double computeTrans(double time, int state1, int state2, double &derv1, double &derv2);

	/**
		compute the transition probability and its 1st and 2nd derivatives between two states at a specific model ID
		One should override this function when defining new model.
		The default is the Juke-Cantor model, valid for all kind of data (DNA, AA, Codon, etc)
		@param time time between two events
		@param model_id model ID
		@param state1 first state
		@param state2 second state
		@param derv1 (OUT) 1st derivative
		@param derv2 (OUT) 2nd derivative
	*/
	virtual double computeTrans(double time, int model_id, int state1, int state2, double &derv1, double &derv2);

	/**
	 * @return pattern ID to model ID map, useful for e.g., partition model
	 * @param ptn pattern ID of the alignment
	 */
	virtual int getPtnModelID(int ptn) { return 0; }
	

	/**
		Get the rate matrix. One should override this function when defining new model.
		The default is equal rate of 1 (JC Model), valid for all kind of data.
		@param rate_mat (OUT) upper-triagle rate matrix. Assume rate_mat has size of num_states*(num_states-1)/2
	*/
	
	virtual void getRateMatrix(double *rate_mat);

	/**
		Get the rate matrix Q. One should override this function when defining new model.
		The default is equal rate of 1 (JC Model), valid for all kind of data.
		@param rate_mat (OUT) upper-triagle rate matrix. Assume rate_mat has size of num_states*(num_states-1)/2
	*/
	virtual void getQMatrix(double *q_mat);

	/**
		compute the state frequency vector. One should override this function when defining new model.
		The default is equal state sequency, valid for all kind of data.
		@param state_freq (OUT) state frequency vector. Assume state_freq has size of num_states
	*/
	virtual void getStateFrequency(double *state_freq);

	/**
		get frequency type
		@return frequency type
	*/
	virtual StateFreqType getFreqType() { return FREQ_EQUAL; }


	/**
		allocate memory for a transition matrix. One should override this function when defining new model
		such as Gamma model. The default is to allocate a double vector of size num_states * num_states. This
		is equivalent to the memory needed by a square matrix.
		@return the pointer to the newly allocated transition matrix
	*/
	virtual double *newTransMatrix();


	/**
		compute the transition probability matrix.and the derivative 1 and 2
		@param time time between two events
		@param trans_matrix (OUT) the transition matrix between all pairs of states. 
			Assume trans_matrix has size of num_states * num_states.
		@param trans_derv1 (OUT) the 1st derivative matrix between all pairs of states. 
		@param trans_derv2 (OUT) the 2nd derivative matrix between all pairs of states. 
	*/
	virtual void computeTransDerv(double time, double *trans_matrix, 
		double *trans_derv1, double *trans_derv2);

	/**
		compute the transition probability matrix.and the derivative 1 and 2 times state frequency vector
		@param time time between two events
		@param trans_matrix (OUT) the transition matrix between all pairs of states. 
			Assume trans_matrix has size of num_states * num_states.
		@param trans_derv1 (OUT) the 1st derivative matrix between all pairs of states. 
		@param trans_derv2 (OUT) the 2nd derivative matrix between all pairs of states. 
	*/
	virtual void computeTransDervFreq(double time, double rate_val, double *trans_matrix, 
		double *trans_derv1, double *trans_derv2);


	/**
		decompose the rate matrix into eigenvalues and eigenvectors
	*/
	virtual void decomposeRateMatrix() {}

	/**
		optimize model parameters. One should override this function when defining new model.
		The default does nothing since it is a Juke-Cantor type model, hence no parameters involved.
		@param epsilon accuracy of the parameters during optimization
		@return the best likelihood 
	*/
	virtual double optimizeParameters(double epsilon) { return 0.0; }

	/**
		write information
		@param out output stream
	*/
	virtual void writeInfo(ostream &out) {}

	/**
		number of states
	*/
	int num_states;

	/**
		name of the model
	*/
	string name;


	/**
		full name of the model
	*/
	string full_name;
	
	/**
	 state frequencies
	 */
	double *state_freq;
	

	/**
		state frequency type
	*/
	StateFreqType freq_type;

	/**
		destructor
	*/
    virtual ~ModelSubst();

protected:

	/**
		this function is served for the multi-dimension optimization. It should pack the model parameters
		into a vector that is index from 1 (NOTE: not from 0)
		@param variables (OUT) vector of variables, indexed from 1
	*/
	virtual void setVariables(double *variables) {}

	/**
		this function is served for the multi-dimension optimization. It should assign the model parameters
		from a vector of variables that is index from 1 (NOTE: not from 0)
		@param variables vector of variables, indexed from 1
	*/
	virtual void getVariables(double *variables) {}

};

#endif
