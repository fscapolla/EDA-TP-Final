#include "Blockchain.h"



void Blockchain::parsingBlockchain(json chain_JData)
{
	/* GETTING BLOCKS */
	for (const auto& data : chain_JData)
	{
		Block tempBlock;

		string BBID = data["blockid"];
		tempBlock.setBlockID(BBID);

		string MKLR = data["merkleroot"];
		tempBlock.setMerkleRoot(MKLR);

		uint NTX = data["nTx"];
		tempBlock.setNtx(NTX);

		int NCE = data["nonce"];
		tempBlock.setNonce(NCE);

		string PBID = data["previousblockid"];
		tempBlock.setPrevBlockID(PBID);

		json transObj = data["tx"];

		/* TRANSACCIONES */
		for (const auto& TXX : transObj)			//Voy dividiendo en pequenos jsons
		{
			Transaction tempTx;

			auto TXD = TXX["nTxin"];
			tempTx.nTxin = TXD;
			cout << tempTx.nTxin << endl;

			auto TXO = TXX["nTxout"];
			tempTx.nTxout = TXO;
			cout << tempTx.nTxout << endl;

			auto TXID = TXX["txid"];
			tempTx.txID = TXID.get<string>();

			cout << tempTx.txID << endl;

			/* VINS */
			json VinObj = TXX["vin"];

			for (const auto& VINdata : VinObj)
			{
				VinS tempVin;

				auto LBID = VINdata["blockid"];
				tempVin.LilblockID = LBID.get<string>();

				cout << LBID << endl;

				auto OUIX = VINdata["outputIndex"];
				tempVin.outputIndex = OUIX;

				cout << OUIX << endl;

				auto SGT = VINdata["signature"];
				tempVin.signature = SGT.get<string>();

				cout << SGT << endl;

				auto TXID = VINdata["txid"];
				tempVin.signature = TXID.get<string>();;

				cout << TXID << endl;

				/* Vin temporario listo para agregar al vector de vins de transaccion temporal*/
				tempTx.vIn.push_back(tempVin);
			}

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
			tempBlock.setTX(tempTx);
		}
		/* Block temporal listo para agregar a nuestro vector de bloques */
		BlocksArr.push_back(tempBlock);
	}
}
