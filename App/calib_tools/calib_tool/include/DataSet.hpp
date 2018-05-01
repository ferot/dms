#ifndef DATASET_HPP
#define DATASET_HPP
#include <QString>

class DataSet {
private:
	/**
	 * Path to file default finding path.
	 */
	QString m_filepath;
	QString m_payload;
	int m_count;


public:

	DataSet() : //TODO : change fixed name into 'dynamic' one (adding postfix f.ex. : train_data_1.dat etc)
			m_filepath("train_data.dat"), m_payload(""), m_count(0) {
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
