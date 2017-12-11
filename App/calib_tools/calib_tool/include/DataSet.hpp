#ifndef DATASET_HPP
#define DATASET_HPP
#include <QString>

class DataSet {
private:
	/**
	 * Path to file default finding path.
	 */
	QString m_filepath;
	int m_count;
	QString m_payload;

public:

	DataSet() :
			m_filepath(""), m_count(0), m_payload("") {
	}

	int getCount() const{
		return m_count;
	}

	QString getPayload() const {
		return m_payload;
	}

	void setPayload(QString str) {
		m_payload = str;
	}

	void incrCount(){
		m_count++;
	}

	QString getFilePath() const{
		return m_filepath;
	}

	friend QDataStream& operator<<(QDataStream& stream, const DataSet& ds) {
		stream << ds.m_count << ds.m_payload;
		return stream;
	}
	friend QDataStream& operator>>(QDataStream& stream, DataSet& ds) {
		stream >> ds.m_count >> ds.m_payload;
		return stream;
	}
};

#endif // DATASET_HPP