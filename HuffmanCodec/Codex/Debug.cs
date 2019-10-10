using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanCodec.Codex
{
	class Debug
	{
		public static double AverageCodeLength(Analyzer a,HuffmanCodex c)
		{
			long length = 0;
			long sum = 0;
			foreach (int i in a.Value.Values)
				sum += i;
			foreach (Block i in c.Codex.Keys)
				length += c.Codex[i].length * a.Value[i];
			return 1.0 * length / sum;
		}
	}
}
