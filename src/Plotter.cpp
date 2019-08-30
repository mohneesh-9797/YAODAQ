#include "Plotter.hpp"


 Plotter::Plotter(Data& da,WsServer& ser):dat(da),server(ser)
  {
  }


void Plotter::PlotWaveform()
	{
		int NbrBins=FindMaxBin();
    float maxValue{0};
    float tickSize{0};
    if(dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) tickSize=dat.WDcfg.Ts;
    else tickSize=(dat.WDcfg.Ts * dat.WDcfg.DecimationFactor/1000);
    maxValue=NbrBins;
    
		for(std::size_t ch=0;ch!=dat.WDcfg.Nch;++ch)
		{
			static int erasetrigger{0};
			std::string title="";
      			if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) && ((ch != 0) && (((ch+1) % 9)==0) ))
			{
				title="Trigger"+std::to_string((int)((ch-1) / 16));
				
			}
			else 
			{
					title="Channel"+std::to_string(erasetrigger);
					
					erasetrigger++;
			}
			
			histos.push_back(TH1D(title.c_str(),title.c_str(),NbrBins,0,maxValue));
			if(ch==dat.WDcfg.Nch-1) erasetrigger=0;
		}
		if (dat.WDcfg.Nbit == 8) 
		{
		  for(std::size_t ch=0;ch!=dat.WDcfg.Nch;++ch)
		  {
		    int Size = dat.Event8->ChSize[ch];
        if (Size <= 0) continue;
        for(int j=0; j<Size; j++)
		    {
			        histos[ch].Fill(j,dat.Event8->DataChannel[ch][j]);
		    }
		  }
    } 
		else if(dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE)
		{
			for(std::size_t gr=0;gr<(dat.WDcfg.Nch/8);gr++) 
			{
        if(dat.Event742->GrPresent[gr])
				{
        	for(std::size_t ch=0; ch<9; ch++) 
					{
            int Size = dat.Event742->DataGroup[gr].ChSize[ch];
            if (Size <= 0) continue;
            for(int j=0; j<Size; j++)
		        {
			        histos[gr*9+ch].Fill(j,dat.Event742->DataGroup[gr].DataChannel[ch][j]);
		        }
          }
       	}
    	}
		}
		else
		{
		  for(std::size_t ch=0;ch!=dat.WDcfg.Nch;++ch)
		  {
		    int Size = dat.Event16->ChSize[ch];
        if (Size <= 0) continue;
        for(int j=0; j<Size; j++)
		    {
			        histos[ch].Fill(j,dat.Event16->DataChannel[ch][j]);
		    }
		  }
		}
  }


void Plotter::SaveFFT()
{
  auto &echo_all = server.endpoint["^/Rack/?$"];
  TH1* toto{nullptr};
  TString json;
  for(std::size_t i=0;i!=histos.size();++i)
		{
		   toto=histos[i].FFT(nullptr,"RE R2C EX");
		   toto->SetTitle(("FFT Real Part "+std::string(histos[i].GetName())).c_str()); 
		   toto->SetName(("RE"+std::string(histos[i].GetName())).c_str()); 
		   json = TBufferJSON::ToJSON(toto);
			 for(auto &a_connection : echo_all.get_connections()) a_connection->send(json.Data());
		   toto=histos[i].FFT(nullptr,"IM R2C EX");
		   toto->SetTitle(("FFT Imaginary Part "+std::string(histos[i].GetName())).c_str()); 
		   toto->SetName(("IM"+std::string(histos[i].GetName())).c_str()); 
		   json = TBufferJSON::ToJSON(toto);
			 for(auto &a_connection : echo_all.get_connections()) a_connection->send(json.Data());
			 toto=histos[i].FFT(nullptr,"MAG R2C EX");
		   toto->SetTitle(("FFT Magnitude "+std::string(histos[i].GetName())).c_str()); 
		   toto->SetName(("MAG"+std::string(histos[i].GetName())).c_str()); 
		   json = TBufferJSON::ToJSON(toto);
			 for(auto &a_connection : echo_all.get_connections()) a_connection->send(json.Data());
			 toto=histos[i].FFT(nullptr,"PH R2C EX");
		   toto->SetTitle(("FFT Phase "+std::string(histos[i].GetName())).c_str()); 
		   toto->SetName(("PH"+std::string(histos[i].GetName())).c_str()); 
		   json = TBufferJSON::ToJSON(toto);
			 for(auto &a_connection : echo_all.get_connections()) a_connection->send(json.Data());
			 
		}
}


	void Plotter::Save()
	{	for(std::size_t i=0;i!=histos.size();++i)
		{
 			//histos[i].SetMarkerStyle(kFullSquare);
			//histos[i].Draw("P0");
			//can.SaveAs(("/home/rpclab/data/"+std::string(histos[i].GetTitle())+".png").c_str());
			TString json = TBufferJSON::ToJSON(&histos[i]);
			auto &echo_all = server.endpoint["^/Rack/?$"];
			for(auto &a_connection : echo_all.get_connections()) a_connection->send(json.Data());
		}
		SaveFFT();
		/*for(std::size_t i=0;i!=histos.size();++i)
		{
 			histos[i].SetMarkerStyle(kFullSquare);
			if(i==0)histos[i].Draw("P0 PLC PMC");
			else histos[i].Draw("SAME P0 PLC PMC");
			
		}
                
		can.SaveAs("/home/rpclab/data/All.png");*/
                
                //auto &echo_all = server.endpoint["^/Rack/?$"];
		//for(auto &a_connection : echo_all.get_connections()) a_connection->send(json.Data());
		histos.clear();
		

	}













