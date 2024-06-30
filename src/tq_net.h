#ifndef TQ_MULTIROPE_H_
#define TQ_MULTIROPE_H_

namespace Tiq {
	template<typename T>
	class MultiRope {
		private:
			class Tree {
				// TODO
			}

		public:
			class VerseId {
				size_t* verse_id_ = nullptr;
			}
			class Anchor {
				public:
					void original_verse();
					void actual_verse();
					void next_verse();
					void prev_verse();
					void next();
					void prev();
					void to(size_t);
					void to_verse(VerseId);

					VerseId& get_verse();
					size_t get_pos();
					bool is_end();

					T& get() const;
					void set(T, VerseId);

					Anchor& operator ++();
					Anchor& operator ++(int);
					Anchor& operator --();
					Anchor& operator --(int);
					Anchor& operator +=(size_t);
					Ahcnor& operator -=(size_t);
					T& operator *() const;
			}

		public:
			MultiRope(unsigned int cluster_size);
			virtual ~MultiRope() = default;

			insert(Anchor&, T, VerseId);
			delete(Anchor&);
			split(Anchor&);
			join(MultiRope<T>&);
			begin();
			begin(VerseId);
			end();
			end(VerseId);
			find(size_t);
			find(size_t, VerseId);

		protected:
			size_t cluster_size_;

			
	}
}

#endif // TQ_MULTIROPE_H_
