using System;
using System.Collections.Generic;
using System.IO;

namespace HuffmanCodec.Codex
{
    /// <summary>
    /// 对应Block出现的概率表
    /// </summary>
    class Analyzer
    {
        private Dictionary<Block, int> value;
        /// <summary>
        /// 对指定的字节流创建概率表
        /// </summary>
        /// <param name="s">包含目标文件的编码流</param>
        public Analyzer(Stream s)
		{
			Block i;
			value = new Dictionary<Block, int>();

			while (s.Position != s.Length)
			{
				i = new Block(s);
				if (Value.ContainsKey(i))
					++Value[i];
				else
					Value.Add(i, 1);
			}
			s.Position = 0;
		}
		/// <summary>
		/// 获取指定值的出现次数
		/// </summary>
		public Dictionary<Block, int> Value
        {
            get
            {
                return value;
            }
        }
        /// <summary>
        /// 获取概率表中元素个数
        /// </summary>
        public int NumberOfKeys
        {
            get
            {
                return Value.Count;
            }
        }
    }
}
