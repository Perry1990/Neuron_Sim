#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define _sim_steps ((150 + 50) * 50 * 4)
#define _step_time 0.25
#define E_plus 0
#define E_minus -70
#define EL_plus -65
#define EL_minus -55
#define GL_plus 0.01
#define GL_minus 1.0
#define T1 0.5
#define T2 2.4
#define T3 1.0
#define T4 7.0
#define T5_FF 32.0
#define T6_FF 16.0
#define T7_FF 6.21
#define T5_FB 256.0
#define T6_FB 16.0
#define T7_FB 9999.0
#define A1_FF 0.024
#define A2_FF 0.022
#define A1_FB -0.04
#define A2_FB 0.4
#define T_refr_plus 50
#define T_refr_minus 1
#define C0 10.0
#define V_thld -51.0
#define V_rst -65.0
#define I_SprThld 1000.0
#define ms_activePeriod 50
#define ms_idlePeriod 150
#define num_presentation 30
#define t_learn 4.0

class channel
{
private:	
	int src, dst;
	double w;
public:
	channel() {};
	~channel() {};
};

class neuron
{
private:
	double V;
	vector<channel *> _input_channels, _output_channels;
public:
	neuron() {};
	~neuron() {};
};

void buildNet()
{
}

double input_map[10][10];
int input_map_timing[10][10];
double V[201];
int mode[201];
double W[201][201];
vector<vector<int> > past_spikes;
bool test_output;

void initInputPattern(int total_step)
{
	int i, j;
	for (i = 0; i < 10; i ++)
		for (j = 0; j < 10; j ++)
			input_map_timing[i][j] = rand() % total_step;
}

void applyInputPattern(int patternNo, int step)
{
	int i, j;
	for (i = 0; i < 10; i ++)
		for (j = 0; j < 10; j ++)
		{
			if (rand() % 6000 == 0)
				input_map[i][j] = 0;
			else
				input_map[i][j] = 0;
		}
	switch(patternNo)
	{
		case 1:
			for (i = 7; i < 10; i ++)
				for (j = 0; j < 3; j ++)
					if (step - input_map_timing[i][j] == 0)
						input_map[i][j] = I_SprThld;
			break;
			
		case 0:
			if (input_map_timing[0][5] == step) input_map[0][5] = I_SprThld;
			if (input_map_timing[1][4] == step) input_map[1][4] = I_SprThld;
			if (input_map_timing[1][6] == step) input_map[1][6] = I_SprThld;
			if (input_map_timing[2][3] == step) input_map[2][3] = I_SprThld;
			if (input_map_timing[2][7] == step) input_map[2][7] = I_SprThld;
			if (input_map_timing[3][2] == step) input_map[3][2] = I_SprThld;
			if (input_map_timing[3][8] == step) input_map[3][8] = I_SprThld;
			if (input_map_timing[4][1] == step) input_map[4][1] = I_SprThld;
			if (input_map_timing[4][9] == step) input_map[4][9] = I_SprThld;
			if (input_map_timing[5][0] == step) input_map[5][0] = I_SprThld;
			break;
			
		case 2:
			for (i = 0; i < 3; i ++)
				for (j = 4; j < 7; j ++)
					if (step == input_map_timing[i][j])
						input_map[i][j] = I_SprThld;
		
		case 3:
			if (input_map_timing[3][6] == step) input_map[3][6] = I_SprThld;
			if (input_map_timing[3][5] == step) input_map[3][5] = I_SprThld;
			if (input_map_timing[4][4] == step) input_map[3][6] = I_SprThld;
			if (input_map_timing[5][4] == step) input_map[5][4] = I_SprThld;
			if (input_map_timing[6][3] == step) input_map[6][3] = I_SprThld;
			if (input_map_timing[7][4] == step) input_map[7][4] = I_SprThld;
			if (input_map_timing[8][5] == step) input_map[8][5] = I_SprThld;
			if (input_map_timing[8][6] == step) input_map[8][6] = I_SprThld;
			break;
			
		case 4:
			if (input_map_timing[3][5] == step) input_map[3][5] = I_SprThld;
			if (input_map_timing[4][5] == step) input_map[4][5] = I_SprThld;
			if (input_map_timing[5][5] == step) input_map[5][5] = I_SprThld;
			if (input_map_timing[6][5] == step) input_map[6][5] = I_SprThld;
			if (input_map_timing[7][5] == step) input_map[7][5] = I_SprThld;
			if (input_map_timing[8][5] == step) input_map[8][5] = I_SprThld;
			if (input_map_timing[9][5] == step) input_map[9][5] = I_SprThld;
			if (input_map_timing[9][6] == step) input_map[9][6] = I_SprThld;
			if (input_map_timing[9][7] == step) input_map[9][7] = I_SprThld;
			break;
		
		case 5:
			if (input_map_timing[4][7] == step) input_map[4][7] = I_SprThld;
			if (input_map_timing[4][3] == step) input_map[4][3] = I_SprThld;
			if (input_map_timing[5][6] == step) input_map[5][6] = I_SprThld;
			if (input_map_timing[5][4] == step) input_map[5][4] = I_SprThld;
			if (input_map_timing[6][5] == step) input_map[6][5] = I_SprThld;
			if (input_map_timing[7][4] == step) input_map[7][4] = I_SprThld;
			if (input_map_timing[7][6] == step) input_map[7][6] = I_SprThld;
			if (input_map_timing[8][3] == step) input_map[8][3] = I_SprThld;
			if (input_map_timing[8][7] == step) input_map[8][7] = I_SprThld;
			break;
		
		case 6:
			if (input_map_timing[2][4] == step) input_map[2][4] = I_SprThld;
			if (input_map_timing[3][3] == step) input_map[3][3] = I_SprThld;
			if (input_map_timing[3][5] == step) input_map[3][5] = I_SprThld;
			if (input_map_timing[4][3] == step) input_map[4][3] = I_SprThld;
			if (input_map_timing[4][5] == step) input_map[4][5] = I_SprThld;
			if (input_map_timing[5][3] == step) input_map[5][3] = I_SprThld;
			if (input_map_timing[5][4] == step) input_map[5][4] = I_SprThld;
			if (input_map_timing[5][5] == step) input_map[5][5] = I_SprThld;
			if (input_map_timing[6][2] == step) input_map[6][2] = I_SprThld;
			if (input_map_timing[6][6] == step) input_map[6][6] = I_SprThld;
			if (input_map_timing[7][2] == step) input_map[7][2] = I_SprThld;
			if (input_map_timing[7][6] == step) input_map[7][6] = I_SprThld;
			break;
		
		case 7:
			if (input_map_timing[2][4] == step) input_map[2][4] = I_SprThld;
			if (input_map_timing[2][3] == step) input_map[2][3] = I_SprThld;
			if (input_map_timing[2][5] == step) input_map[2][5] = I_SprThld;
			if (input_map_timing[3][3] == step) input_map[3][3] = I_SprThld;
			if (input_map_timing[3][5] == step) input_map[3][5] = I_SprThld;
			if (input_map_timing[4][3] == step) input_map[4][3] = I_SprThld;
			if (input_map_timing[4][5] == step) input_map[4][5] = I_SprThld;
			if (input_map_timing[5][3] == step) input_map[5][3] = I_SprThld;
			if (input_map_timing[5][4] == step) input_map[5][4] = I_SprThld;
			if (input_map_timing[5][5] == step) input_map[5][5] = I_SprThld;
			if (input_map_timing[6][3] == step) input_map[6][3] = I_SprThld;
			if (input_map_timing[6][5] == step) input_map[6][5] = I_SprThld;
			break;
			
		case 8:
			if (input_map_timing[2][4] == step) input_map[2][4] = I_SprThld;
			if (input_map_timing[2][3] == step) input_map[2][3] = I_SprThld;
			if (input_map_timing[3][3] == step) input_map[3][3] = I_SprThld;
			if (input_map_timing[3][4] == step) input_map[3][4] = I_SprThld;
			if (input_map_timing[4][3] == step) input_map[4][3] = I_SprThld;
			if (input_map_timing[4][5] == step) input_map[4][5] = I_SprThld;
			if (input_map_timing[5][3] == step) input_map[5][3] = I_SprThld;
			if (input_map_timing[5][4] == step) input_map[5][4] = I_SprThld;
			if (input_map_timing[5][5] == step) input_map[5][5] = I_SprThld;
			if (input_map_timing[5][6] == step) input_map[5][6] = I_SprThld;
			if (input_map_timing[6][2] == step) input_map[6][2] = I_SprThld;
			if (input_map_timing[6][6] == step) input_map[6][6] = I_SprThld;
			break;
			
		default:
			break;
	}
}

void clearInputPattern()
{
	int i, j;
	for (int i = 0; i < 10; i ++)
		for (int j = 0; j < 10; j ++)
			input_map[i][j] = 0;
}

double synap_cond(int _step_delta, double Ta, double Tb)
{
	double Tab = (Ta * Tb) / (Ta - Tb) * log(Ta / Tb);
	double s = (double)_step_delta / (double)4.0;
	double numerator = exp(-s/Ta) - exp(-s/Tb);
	double denominator = exp(-Tab/Ta) - exp(-Tab/Tb);
	return numerator / denominator;
}

void updateNeurons(int _step)
{
	for (int i = 0; i < 200; i ++)
	{
		double excit_i = 0;
		double inhib_i = 0;
		double ext_i = 0;
		double delta_V = 0;
		double refr_time;
		double cond;
		switch(mode[i])
		{
			case 0:
				
				for (int j = 0; j < 200; j ++)
					if (W[j][i] == 0 || i == j) continue;
					else {
						for (int k = past_spikes[j].size() - 1; k >= 0; k --)
						{
							cond = synap_cond(_step - past_spikes[j][k], T1, T2);
							excit_i += W[j][i] * cond;
							if (abs(W[j][i] * cond < 0.001))
								break;
						}
					}
				excit_i *= (E_plus - V[i]);
				
				if (i >= 100 && i != 200)
				{
					for (int k = past_spikes[200].size() - 1; k >= 0; k --)
					{
						cond = synap_cond(_step - past_spikes[200][k], T3, T4);
						inhib_i += W[200][i] * cond;
						if (abs(W[200][i] * cond) < 0.001)
							break;
					}
				}
				inhib_i *= (E_minus - V[i]);
				//if (test_output && _step == past_spikes[200].back())
					//cout << "inh: " << inhib_i << " ";
				
				if (i < 100)
					ext_i = input_map[i/10][i%10];
				
				if (i != 200)
					delta_V = GL_plus * (EL_plus - V[i]) + excit_i + inhib_i + ext_i;
				else
					delta_V = GL_minus * (EL_minus - V[i]) + excit_i + inhib_i + ext_i;
				delta_V /= C0;
				delta_V /= 1.0;
				
				V[i] += delta_V;
				
				if (test_output)
				{
					cout << V[i] << "(" << delta_V << ")" << " ";
					if (i % 10 == 9) cout << endl;
				}
				
				if (V[i] > V_thld)
				{
					V[i] = V_rst;
					mode[i] = 1;
					past_spikes[i].push_back(_step);
					if (i >= 100 && (past_spikes[200].size() && past_spikes[200].back() != _step || !past_spikes[200].size()))
					{
						past_spikes[200].push_back(_step);
						if (test_output)
							cout << "inhibitor fires" << endl;
					}
				}
				break;
			
			case 1:
				refr_time = (double)(_step - past_spikes[i].back()) * 0.25;
				if (i < 200 && refr_time >= T_refr_plus)
					mode[i] = 0;
				else if (i == 200 && refr_time >= T_refr_minus)
					mode[i] = 0;
					
				if (test_output)
				{
					cout << 0 << " ";
					if (i % 10 == 9) cout << endl;
				}
				
				break;
			
			default:
				cout << "Error: Mode of " << i << " does not exist!" << endl;
				break;
		}
	}
	if (test_output)
		cout << "=============" << endl;
}

void updateSynapse(int _step)
{
	double delta_w[201][201];
	for (int i = 0; i < 200; i ++)
		for (int j = 0; j < 200; j ++)
			delta_w[i][j] = 0;
	for (int i = 0; i < 100; i ++)
		if (past_spikes[i].back() == _step)
			for (int j = 100; j < 200; j ++)
			{
				if (i == j) continue;
				for (int k = past_spikes[j].size() - 1; k >= 0; k --)
				{
					if (_step - past_spikes[j][k] > 200) break;
					if (_step > past_spikes[j][k])
					{
						delta_w[i][j] += A2_FF * exp(-(_step - past_spikes[j][k]) / T6_FF / t_learn);
						delta_w[j][i] += A1_FB * (exp(-(_step - past_spikes[j][k]) / T5_FB / t_learn));
						//cout << "E1 -> E2: " << delta_w[i][j] << endl;
					}
					else
					{
						delta_w[i][j] += A1_FF * (exp(-(past_spikes[j][k] - _step) / T5_FF / t_learn) - exp(-T7_FF));
						delta_w[j][i] += A2_FB * (exp(-(past_spikes[j][k] - _step) / T6_FB / t_learn));
					}
				}
			}
	for (int i = 100; i < 200; i ++)
		if (past_spikes[i].back() == _step)
			for (int j = 0; j < 100; j ++)
			{
				if (i == j) continue;
				for (int k = past_spikes[j].size() - 1; k >= 0; k --)
				{
					if (_step - past_spikes[j][k] > 200) break;
					if (_step > past_spikes[j][k])
					{
						delta_w[i][j] += A2_FB * exp(-(_step - past_spikes[j][k]) / T6_FB / t_learn);
						delta_w[j][i] += A1_FF * (exp(-(_step - past_spikes[j][k]) / T5_FF / t_learn) - exp(-T7_FF));
						//cout << "E2 -> E1: " << delta_w[i][j] << endl;
					}
					else
					{
						delta_w[i][j] += A1_FB * (exp(-(past_spikes[j][k] - _step) / T5_FB / t_learn));
						delta_w[j][i] += A2_FF * exp(-(past_spikes[j][k] - _step) / T6_FF / t_learn);
					}
				}
			}
	
	double total_W_old = 0, total_W = 0;
	for (int i = 100; i < 200; i ++)
	{
		total_W_old = 0; total_W = 0;
		for (int j = 0; j < 100; j ++)
		{
			total_W_old += W[j][i];
			W[j][i] += delta_w[j][i];
			if (W[j][i] > 0.4 && j < 100)
				W[j][i] = 0.4;
			if (W[j][i] < 0)
				W[j][i] = 0;
			total_W += W[j][i];
		}
		for (int j = 0; j < 100; j ++)
			W[j][i] *= total_W_old / total_W;
		for (int j = 0; j < 100; j ++)
		{
			if (W[j][i] > 0.15)
				W[j][i] = 0.15;
		}
	}
	
	for (int i = 100; i < 200; i ++)
		for (int j = 0; j < 100; j ++)
		{
			W[i][j] += delta_w[i][j];
			if (W[i][j] > 4 && i >= 100)
				W[i][j] = 4;
			if (W[i][j] < 0)
				W[i][j] = 0;
		}
}

void displayInfo(bool fileWrite)
{
	double max_w = 0;
	int max_w_i = 0;
	vector<int> memory_cell;
	cout << "Synapses Weight Map:" << endl;
	cout << "E2 projective field" << endl;
	for (int i = 100; i < 200; i ++)
	{
		double sum_w = 0;
		for (int j = 0; j < 100; j ++)
			sum_w += W[i][j];
		cout << sum_w << " ";
		if (i % 10 == 9)
			cout << endl;
		if (sum_w > max_w)
		{
			max_w = sum_w;
			max_w_i = i;
		}
		if (sum_w > 2)
			memory_cell.push_back(i);
	}
	cout << "E2 receptive field" << endl;
	for (int i = 100; i < 200; i ++)
	{
		double sum_w = 0;
		for (int j = 0; j < 100; j ++)
			sum_w += W[j][i];
		cout << sum_w << " ";
		if (i % 10 == 9)
			cout << endl;
		if (sum_w > max_w)
		{
			max_w = sum_w;
			max_w_i = i;
		}
	}
	cout << "E1 projective field" << endl;
	for (int i = 0; i < 100; i ++)
	{
		double sum_w = 0;
		for (int j = 100; j < 200; j ++)
			sum_w += W[i][j];
		cout << sum_w << " ";
		if (i % 10 == 9)
			cout << endl;
	}
	cout << "The synapse with largest weight, E1 -> E2:" << endl;
	for (int i = 0; i < 100; i ++)
	{
		cout << W[i][max_w_i] << " ";
		if (i % 10 == 9)
			cout << endl;
	}
	cout << "Number of past spikes of each cell in E2: " << endl;
	for (int i = 100; i < 200; i ++)
	{
		cout << past_spikes[i].size() << " ";
		if (i % 10 == 9)
			cout << endl;
	}
	cout << "E2 -> E1 synapses weight of notable cells in E2: " << endl;
	for (int i = 0; i < memory_cell.size(); i ++)
	{
		for (int j = 0; j < 100; j ++)
		{
			cout << W[memory_cell[i]][j] << " ";
			if (j % 10 == 9)
				cout << endl;
		}
		cout << "=========================" << endl;
	}
	if (memory_cell.size() == 0)
	{
		for (int i = 0; i < 100; i ++)
		{
			cout << W[max_w_i][i] << " ";
			if (i % 10 == 9)
				cout << endl;
		}
		cout << "=========================" << endl;
	}
	if (!fileWrite)
		return;
	ofstream output1, output2;
	output1.open("E2projective.txt");
	output2.open("E2receptive.txt");
	for (int i = 100; i < 200; i ++)
		for (int j = 0; j < 100; j ++)
		{
			output1 << W[i][j] << " ";
			output2 << W[j][i] << " ";
			if (j % 10 == 9)
			{
				output1 << endl;
				output2 << endl;
			}
		}
	output1.close();
	output2.close();
}

void learningModeSim()
{
	srand(7);
	test_output = false;
	cout << std::setprecision(2) << std::fixed;
	
	for (int i = 0; i < 100; i ++)
	{
		V[i] = V_rst;
		W[i][200] = 0;
		W[200][i] = 0;
		for (int j = 0; j < 100; j ++)
			W[i][j] = 0;
		for (int j = 100; j < 200; j ++)
			W[i][j] = (double)(rand() % 4) * 0.01 + 0.02;
	}
	for (int i = 100; i < 200; i ++)
	{
		V[i] = V_rst;
		W[i][200] = 2;
		W[200][i] = 10;
		for (int j = 0; j < 200; j ++)
			W[i][j] = 0;
	}
	for (int i = 0; i <= 200; i ++)
	{
		mode[i] = 0;
		vector<int> past_spike_node;
		past_spike_node.clear();
		past_spikes.push_back(past_spike_node);
	}
	int _step = 0;
	for (int i = 0; i < num_presentation; i ++)
	{
		int inj_time = rand() % (20 * 4);
		
		/*if (i == 10)
			test_output = true;
		else test_output = false;*/
		
		cout << i << "th learning process" << endl;
		initInputPattern(ms_activePeriod * 4);
		for (int j = 0; j < ms_activePeriod * 4; j ++)
		{
			/*if (j > 0 && j <= 50 && i == 10)
				test_output = true;
			else test_output = false;*/
			
			applyInputPattern(6, j);
			updateNeurons(_step);
			updateSynapse(_step);
			if (j == inj_time)
				clearInputPattern();
			_step ++;
		}
		
		test_output = false;
		
		for (int j = 0; j < ms_idlePeriod * 4; j ++)
		{
			updateNeurons(_step);
			updateSynapse(_step);
			_step ++;
		}
	}
	//displayInfo();
	
	for (int i = 0; i < num_presentation; i ++)
	{
		int inj_time = rand() % (20 * 4);
		
		/*if (i == 10)
			test_output = true;
		else test_output = false;*/
		
		cout << i << "th learning process" << endl;
		initInputPattern(ms_activePeriod * 4);
		for (int j = 0; j < ms_activePeriod * 4; j ++)
		{
			/*if (j > 50 && j <= 100 && i == 10)
				test_output = true;
			else test_output = false;*/
			
			applyInputPattern(7, j);
			updateNeurons(_step);
			updateSynapse(_step);
			if (j == inj_time)
				clearInputPattern();
			_step ++;
		}
		for (int j = 0; j < ms_idlePeriod * 4; j ++)
		{
			updateNeurons(_step);
			updateSynapse(_step);
			_step ++;
		}
	}
	
	//displayInfo();
	
	for (int i = 0; i < num_presentation; i ++)
	{
		int inj_time = rand() % (20 * 4);
		
		cout << i << "th learning process" << endl;
		initInputPattern(ms_activePeriod * 4);
		for (int j = 0; j < ms_activePeriod * 4; j ++)
		{
			
			applyInputPattern(8, j);
			updateNeurons(_step);
			updateSynapse(_step);
			if (j == inj_time)
				clearInputPattern();
			_step ++;
		}
		for (int j = 0; j < ms_idlePeriod * 4; j ++)
		{
			updateNeurons(_step);
			updateSynapse(_step);
			_step ++;
		}
	}
}

void testModeSim()
{
}

int main()
{
	buildNet();
	learningModeSim();
	displayInfo(true);
	testModeSim();
	return 0;
}
