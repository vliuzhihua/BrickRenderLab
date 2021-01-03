#pragma once

#include <string>
#include <algorithm>
#include <initializer_list>

namespace brick {

namespace math{

	template<int Size, typename T>
	class Vector {
	public:
		T m_data[Size] = { 0.0 };

		Vector(){}

		Vector(T data[]) {
			memcpy(m_data, data, sizeof(T) * Size);
		}

		T& operator[](int idx) {
			return m_data[idx];
		}

		T operator[](int idx) const {
			return m_data[idx];
		}

		template<int Size2>
		void operator = (const Vector<Size2, T>& other) {
			constexpr int MinSize = std::min(Size, Size2);
			for (int i = 0; i < MinSize; i++) {
				m_data[i] = other[i];
			}
		}

		Vector operator * (float scaler) const {
			T data[Size];
			for (int i = 0; i < Size; i++) 
				data[i] = m_data[i] * scaler;
			return data;
		}

		Vector operator + (float scaler) const {
			T data[Size];
			for (int i = 0; i < Size; i++) 
				data[i] = m_data[i] + scaler;
			return data;
		}

		//template<int Size2, typename T2>
		//Vector operator + (const Vector<Size2, T2>& vec) const {
		//	constexpr int MaxSize = std::max(Size, Size2);
		//	T data[MaxSize];
		//	if constexpr (Size > Size2) {
		//		for (int i = 0; i < Size2; i++) 
		//			data[i] = m_data[i] + vec[i];
		//		for(int i = Size2; i < Size; i++)
		//			data[i] = m_data[i]
		//	}
		//	else {
		//		for (int i = 0; i < Size; i++) 
		//			data[i] = m_data[i] + vec[i];
		//		for(int i = Size; i < Size2; i++)
		//			data[i] = vec[i]
		//	}
		//	return data;
		//}

		Vector operator + (const Vector& vec) const {
			constexpr int MaxSize = std::max(Size, Size2);
			T data[Size];
			for (int i = 0; i < Size; i++)
				data[i] = m_data[i] + vec[i];
			return data;
		}

		T Dot(const Vector& vec) const {
			T Result = 0.0;
			for (int i = 0; i < Size; i++)
				Result += m_data[i] * vec[i];
			return Result;
		}

		//Vector Cross(const Vector& other) const {
		//	static_assert(false);
		//}

	};

	template<>
	class Vector<3, float> {
	public:
		union{
			struct{
				float m_data[3];
			};
			struct {
				float x, y, z;
			};
			struct {
				float r, g, b;
			};
		};

		Vector() {
			x = y = z = 0.0f;
		}

		Vector(float s) {
			x = y = z = s;
		}

		Vector(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector(float data[3]) {
			memcpy(m_data, data, sizeof(float) * 3);
		}

		float& operator[](int idx) {
			return m_data[idx];
		}

		float operator[](int idx) const {
			return m_data[idx];
		}

		Vector Cross(const Vector& other) const {
			return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
		}

		Vector operator + (float scaler) const {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = m_data[i] + scaler;
			return data;
		}

		Vector operator + (const Vector& vec) const {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = m_data[i] + vec[i];
			return data;
		}

		Vector operator * (float scaler) const {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = m_data[i] * scaler;
			return data;
		}

		friend Vector operator * (float scaler, const Vector& vec) {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = vec[i] * scaler;
			return data;
		}

		std::string GetString() {
			std::string Result = "";
			char Buf[64];
			for (int i = 0; i < 3; i++) {
				sprintf(Buf, "%f", m_data[i]);
				Result += Buf;
				Result += " ";
			}
			return Result;
		}

		const float* GetData() {
			return m_data;
		}

	};

	//column major
	template<int N, int M, typename T>
	class Matrix {
	public:
		
		static Matrix Identity() {
			//constexpr int V = std::min(N, M);
			int V = std::min<int>(N, M);
			T data[N * M] = { 0.0f };
			for (int i = 0; i < V; i++) {
				data[i * M + i] = 1.0f;
			}
			return data;
		}

		Matrix(){}

		Matrix(const T data[]) {
			//static_assert(sizeof(data) == N * M * sizeof(T));
			int a = sizeof(T) * N * M;
			memcpy(m_data, data, sizeof(T) * N * M);
		}

		Matrix(std::initializer_list<T> args) {
			int i = 0;
			for (auto iter = args.begin(); iter != args.end(); ++iter, i++)
				m_data[i] = *iter;
		}

		//T& operator[](int n) {
		//	return m_data[n * M + c];
		//}

		T& operator() (int n, int c) {
			return m_data[n * M + c];
		}

		T operator() (int n, int c) const {
			return m_data[n * M + c];
		}
			
		template<int M2>
		Matrix operator * (const Matrix<M, M2, T>& other) {
			const T* other_data = other.GetData();
			T data[N * M2] = { 0.0 };
			for (int r = 0; r < N; r++)
				for (int c = 0; c < M; c++) {
					int idx = r * M + c;
					for (int i = 0; i < M; i++)
						data[idx] += m_data[r * M + i] * other_data[i * M2 + c];
				}
			return data;
		}

		Vector<N, T> operator * (const Vector<M, T> other) {
			T data[N] = {0.0};
			for(int r = 0; r < N; r++)
				for (int c = 0; c < M; c++) {
					data[r] += m_data[r * M + c] * other[c];
				}
			return data;
		}

		Matrix<M, N, T> Transpose() {
			T data[N * M] = { 0.0f };
			for (int r = 0; r < M; r++) 
				for (int c = 0; c < N; c++) {
					data[r * N + c] = m_data[c * M + r];
				}
			return data;
		}

		std::string GetString() {
			std::string Result = "";
			char Buf[64];
			for (int r = 0; r < N; r++) {
				for (int c = 0; c < M; c++) {
					sprintf(Buf, "%f", m_data[c * M + r]);
					Result += Buf;
					Result += " ";
				}
				Result += "\n";
			}
			return Result;
		}

		const T* GetData() const {
			return m_data;
		}
	private:
		T m_data[N * M] = { 0.0 };
	};

	using Vector3f = Vector<3, float>;
	using Vector4f = Vector<4, float>;
	using Matrix4x4f = Matrix<4, 4, float>;
	
	


}

}

