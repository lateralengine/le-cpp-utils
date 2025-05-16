#pragma once

#include <exception>

namespace le
{

	// median assembled from https://stackoverflow.com/questions/1719070/what-is-the-right-approach-when-using-stl-container-for-median-calculation
	
	class median_of_empty_list_exception : public std::exception
	{
		virtual const char* what() const throw() {
			return "Attempt to take the median of an empty list of numbers.  "
				"The median of an empty list is undefined.";
		}
	};

	template<class RandAccessIter>
	float median(RandAccessIter begin, RandAccessIter end)
	{
		if (begin == end) { throw median_of_empty_list_exception(); }
		std::size_t size = end - begin;
		std::size_t middleIdx = size / 2;
		RandAccessIter target = begin + middleIdx;
		std::nth_element(begin, target, end);

		float med = static_cast<float>(*target);
		if (size % 2 == 0) // Even number of elements
		{
			auto max_it = std::max_element(begin, begin + middleIdx);
			med = (*max_it + med) / 2.f;
		}
		return med;
	}

}