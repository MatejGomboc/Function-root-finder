#include <iostream>
#include <random>
#include <set>
#include <cmath>


class candidate
{
public:
	candidate(const double pos = 0.0, const double div = 0.0):
		m_pos(pos),
		m_div(div)
	{
	}

	double m_pos;
	double m_div;
	static const double m_interval;
	static const double m_max_div;

	bool operator<(const candidate& rhs) const
	{
		return m_pos < rhs.m_pos;
	}

	bool operator>(const candidate& rhs) const
	{
		return m_pos > rhs.m_pos;
	}

	bool operator==(const candidate& rhs) const
	{
		return m_pos == rhs.m_pos;
	}

	bool operator!=(const candidate& rhs) const
	{
		return m_pos != rhs.m_pos;
	}

	bool operator<=(const candidate& rhs) const
	{
		return m_pos <= rhs.m_pos;
	}

	bool operator>=(const candidate& rhs) const
	{
		return m_pos >= rhs.m_pos;
	}
};

const double candidate::m_interval = 1e-10;
const double candidate::m_max_div = 1e-3;


double calculate_div(const double pos)
{
	// x^3 - 5x^2 + 8x - 4 = 0
	return fabs(pos * pos * pos - 5.0 * pos * pos + 8.0 * pos - 4.0);
}


int main()
{
	std::random_device generator; // use a hardware entropy source if available, otherwise use PRNG
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	std::set<candidate> candidates;

	for (int i = 0; i < 1000; i++)
	{
		candidate cand = candidate(distribution(generator));
		cand.m_div = calculate_div(cand.m_pos);

		if (cand.m_div < candidate::m_max_div)
		{
			bool found = false;

			for (candidate compr : candidates)
			{
				if (fabs(compr.m_pos - cand.m_pos) < candidate::m_interval / 2.0)
				{
					if (compr.m_div > cand.m_div)
					{
						candidates.erase(compr);
						candidates.emplace(cand);
						found = true;
					}
				}
			}

			if (!found) candidates.emplace(cand);
		}
	}

	for (candidate cand : candidates)
	{
		std::cout << cand.m_div << " " << cand.m_pos << std::endl;
	}

	return 0;
}
