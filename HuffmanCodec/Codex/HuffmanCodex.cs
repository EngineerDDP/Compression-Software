using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanCodec.Codex
{

    /// <summary>
    /// 哈夫曼树节点
    /// </summary>
    class HuffmanCodex
    {
        const int LEFTNODE = 0;
        const int RIGHTNODE = 1;
        private Dictionary<Block, Codec> codex;
		private Dictionary<Codec, Block> dcodex;
        private struct HfTree
        {
            public int Weight
            {
                get;
                set;
            }
            public int Parent
            {
                get;
                set;
            }
            public int LChild
            {
                get;
                set;
            }
            public int RChild
            {
                get;
                set;
            }
        }

		public HuffmanCodex(Byte[] buffer)
		{
			Block key;
			Codec value;
			codex = new Dictionary<Block, Codec>();
			while (buffer.Length != 0)
			{
				key = new Block(buffer);
				buffer = buffer.Skip(Block.BlockSize).ToArray();
				value = new Codec(buffer.Take(Codec.SizeofThis).ToArray());
				buffer = buffer.Skip(Codec.SizeofThis).ToArray();
				codex.Add(key, value);
			}
		}

		/// <summary>
		/// 通过Analyzer创建新的哈夫曼编码表
		/// </summary>
		/// <param name="analyzer">指定的概率分析表</param>
		public HuffmanCodex(Analyzer analyzer)
        {
            Int32 j = analyzer.NumberOfKeys * 2 - 1, i = 0;
            HfTree[] tree = new HfTree[j];
            HfTree node = new HfTree();
			Codec temp;
            codex = new Dictionary<Block, Codec>();
            for (i = 0; i < analyzer.NumberOfKeys; ++i)		//遍历概率表并将权值导入哈夫曼树，将对应的字符导入编码表
            {
                codex.Add(analyzer.Value.ElementAt(i).Key, new Codec(0, 0));
                tree[i].Weight = analyzer.Value.ElementAt(i).Value;
                tree[i].LChild = -1;
                tree[i].RChild = -1;
                tree[i].Parent = -1;
            }
            Huffman(tree, analyzer.NumberOfKeys);		//生成哈夫曼树
			for (i = 0; i < analyzer.NumberOfKeys; ++i)		//遍历所有的叶子节点
            {
				j = i;
				node = tree[j];
				temp = new Codec(0, 0);		//生成哈夫曼编码结构
                while(node.Parent != -1)	//从叶子节点回溯到树根
                {
					temp.AddBitFront(tree[node.Parent].LChild == j ? LEFTNODE : RIGHTNODE);  //该节点是其父节点的左节点加一位0，右节点加一位1
					j = node.Parent;		//回溯父节点
                    node = tree[j];
				}
				codex[analyzer.Value.ElementAt(i).Key] = temp;		//保存该节点的编码值
            }
        }

        /// <summary>
        /// 通过给出的叶子节点创建哈夫曼树
        /// </summary>
        /// <param name="tree">数组表示的哈夫曼树</param>
        /// <param name="m">叶子节点个数</param>
        private void Huffman(HfTree[] tree, int m)
        {
            Int32 i = 0, j = 0;
            Int32 m1 = Int32.MaxValue;		//最小节点值
            Int32 m2 = Int32.MaxValue;		//次小节点值
            Int32 x1 = -1;					//最小节点下标
            Int32 x2 = -1;					//次小节点下标
            for (i = 0; i < m - 1; ++i)		//循环遍历哈夫曼树，直到最后一个叶子节点
            {
				m1 = Int32.MaxValue;
				m2 = Int32.MaxValue;
				for (j = 0; j < m + i; ++j)		//遍历节点，找到权值最小的两个
                {
                    if (tree[j].Weight < m1 && tree[j].Parent == -1)	//寻找一个最小的节点,并将原最小节点的值赋值给次小节点
                    {
                        m2 = m1;
                        m1 = tree[j].Weight;
                        x2 = x1;
                        x1 = j;
                    }
                    else if (tree[j].Weight < m2 && tree[j].Parent == -1)	//若有一个节点大于最小节点但小于次小节点,将其赋值给次小节点
                    {
                        m2 = tree[j].Weight;
                        x2 = j;
                    }
                }
                tree[x1].Parent = m + i;		//设置两个最小节点的父节点
                tree[x2].Parent = m + i;
                tree[m + i].Weight = tree[x1].Weight + tree[x2].Weight;		//设置父节点权值
                tree[m + i].LChild = x1;
                tree[m + i].RChild = x2;
				tree[m + i].Parent = -1;
            }
        }
        /// <summary>
        /// 获取哈夫曼编码表
        /// </summary>
        public Dictionary<Block, Codec> Codex
        {
            get
            {
                return codex;
            }
        }
		/// <summary>
		/// 尝试依照编码寻找对应的字符
		/// </summary>
		/// <param name="value"></param>
		/// <param name="c"></param>
		/// <returns></returns>
		public bool TryGetKeyByValue(Codec value, out Block c)
		{
			if(dcodex == null)
			{
				dcodex = new Dictionary<Codec, Block>();
				foreach (Block key in codex.Keys)
					dcodex.Add(codex[key], key);
			}
			return dcodex.TryGetValue(value, out c);
		}
		/// <summary>
		/// 编码长度
		/// </summary>
		public Int32 Length
		{
			get
			{
				return Codex.Keys.Count * (Block.BlockSize + Codec.SizeofThis) ;
			}
		}
    }
}
