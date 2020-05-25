

//Block::Block(const json & j)
//{
//	/*Cargo los datos del bloque*/
//	height = j["height"].get<unsigned int>();
//	BigBlockID = j["blockid"].get<string>();
//	prevBlockID = j["previousblockid"].get<string>();
//	ntx = j["nTx"].get<unsigned int>();
//	nonce = j["nonce"].get<unsigned int>();
//
//	//Cargo el arreglo de transacciones
//	auto TxInfo= j["tx"];
//	for (auto& tx_ : TxInfo)
//	{
//		Transaction tempTx;
//		auto nTxin_ = tx_["nTxin"].get<unsigned int>();
//		tempTx.nTxin = nTxin_;
//		auto nTxout_ = tx_["nTxout"].get<unsigned int>();
//		tempTx.nTxout = nTxout_;
//		auto txID_ = tx_["txid"].get<string>();
//		tempTx.txID = txID_;
//		auto Vin_ = tx_["vin"];
//		for (auto& vin_ : Vin_)
//		{
//			VinS tempVin;
//
//			auto LilBlockId_ = vin_["blockid"].get<string>();
//			tempVin.LilblockID = LilBlockId_;
//			auto outputIndex_ = vin_["outputIndex"].get<int>();
//			tempVin.outputIndex = outputIndex_;
//			auto signature_ = vin_["signature"].get<string>();
//			tempVin.signature = signature_;
//			auto txid_ = vin_["txid"].get<string>();
//			tempVin.txID = txid_;
//
//			tempTx.vIn.push_back(tempVin);
//		}
//		auto Vout_ = tx_["vout"];
//		for (auto& vout_ : Vout_)
//		{
//			VoutS tempVout;
//
//			auto amount_ = vout_["amount"].get<unsigned int>();
//			tempVout.amount = amount_;
//			auto publicID_ = vout_["publicid"].get<string>();
//			tempVout.publicID = publicID_;
//
//			tempTx.vOut.push_back(tempVout);
//		}
//
//		TxVector.push_back(tempTx);
//	}
//}










	///* VOUTS */
			//json VoutObj = TXX["vout"];

			//for (const auto& VOUTdata : VoutObj)
			//{
			//	VoutS tempVout;
			//	auto AMNT = VOUTdata["amount"];
			//	tempVout.amount = AMNT;
			//	cout << AMNT << endl;


			//	auto PBID = VOUTdata["publicid"];
			//	tempVout.publicID = PBID.get<string>();
			//	cout << PBID << endl;

			//	/* Vout temporario listo para agregar al vector de vouts de transaccion temporal*/
			//	tempTx.vOut.push_back(tempVout);
			//}

			/* Transacciones listas para agregar a bloque temmportal*/
			//tempBlock.setTX(tempTx);












