#include<iostream>
#include<cstdlib>
#include<cstring>
#include<iomanip>
#include<unistd.h>
#include<cstdio>
#include<time.h>

using namespace std;

class team
{
public:
	char tname[30];
	char play[10];
	int score;
	int side;
	
};

int strcomp(char s1[],char s2[])
{
	int i=0;
	for(i=0;s2[i]!='\0';i++)
	{
		if(s1[i]!=s2[i])
			return 1;
	}
	return 0;
}

void strcopy(char s1[],char s2[])
{
	int i=0;
	for(i=0;s2[i]!='\0';i++)
		s1[i]=s2[i];
	s1[i]='\0';
}

	char dribble[16][10];		//Number sequences for skill moves while dribbling
	

	char comm_dribble[15][200];		//Comments regarding different kinds of dribbles

	

	char shot[16][5];			//Character sequences for different types of shots
	

	char comm_shot[15][200];		//Comments regarding the various kinds of shots
	

	char fk[5][4];				//Character Sequences for different types of of free kicks
	

	char comm_fk[5][200];		//Comments on different kinds of free kicks
	


	team tu,tc,tpos;					//tu=team user, tc=team CPU, tpos=team having possession
	int maximum=0,cscore=16;		//maximum=Maximum number of goals required to score for winning, cscore=current score
	char toss,toss_result;		//toss=Head or tail chosen by user, toss_result = Result of toss(head or tail)
	char previous[10];				//to store the previousious move data for helping CPU (AI)

	int flag=0;					//flag variable to determine current status of CPU : 
								//0 for passing
								//1 for shooting or saving a goal
								//2 for penalty
								//3 for free kick
	int w[10];					//Weighted Probability of certain elements to occur..read the code to understand
	int q=0;					//Pivot Variable for deciding which comment to give in which case

//Function for commenting in dribbling :


//toss function:

void toss_func()
{
	cout<<"Choose : Head or Tail (H/T)? \n";
	cin>>toss;
	
	int i=rand()%2;									//0 means Head. 1 means Tail
	if(i==0)	toss_result='H';
	else 		toss_result='T';
			
	if(toss==toss_result)
		{
			cout<<"You won the toss..!!\n";
			cout<<"Which side do you take (0 or 32) ? : ";
			cin>>tu.side;
			if(tu.side==32)	
				tc.side=0;
			else
				tc.side=32;
			tpos=tc;
		}
	else
		{
			cout<<"Oops..!! You Lost the toss\n";
			tc.side=(rand()%2==0)?0:32;
			cout<<tc.tname<<" Chose to take "<<tc.side<<" as side\n";
			if(tc.side==0)
				tu.side=32;
			else
				tu.side=0;
			tpos=tu;
		}	
}

//display function:

void display()
{
	cout<<setw(30)<<setfill(' ');
	cout<<"|"<<setw(10)<<left<<"   Score"<<"|";
	cout<<setw(30)<<setfill(' ');
	cout<<endl;
	cout<<setw(29)<<right<<tu.tname<<"|"<<setw(10)<<left<<tu.score<<"    "<<"-   "<<tc.score<<"|"<<setw(30)
	<<left<<tc.tname;
	
	cout<<"\n\n";

	cout<<setw(30)<<left<<setfill(' ')<<"          Possession";
	cout<<"|"<<left<<setw(30)<<setfill(' ')<<"        Current Number";
	cout<<"|"<<endl;
	for(int i=0;i<65;i++)	cout<<"-";
	cout<<endl;
	int l=strlen(tpos.tname);
	for(int i=0;i<((30-l)/2);i++)
		cout<<" ";
	cout<<left<<setw(30)<<setfill(' ')<<tpos.tname;
	cout<<left<<setw(30)<<setfill(' ')<<cscore;

	cout<<"\n\n";

	cout<<setw(30)<<left<<setfill(' ')<<"          Your Play";
	cout<<"|"<<left<<setw(30)<<setfill(' ')<<"       Opponent's Play";
	cout<<"|"<<endl;
	for(int i=0;i<65;i++)	cout<<"-";
	cout<<endl;
	l=strlen(tu.play);
	for(int i=0;i<((30-l)/2);i++)
		cout<<" ";
	cout<<left<<setw(30)<<setfill(' ')<<tu.play;
	cout<<left<<setw(30)<<setfill(' ')<<tc.play;

	
}


//Function to generate CPU response according to a given weighted probability :

void generate(int n)		//n=No. of elements
{
	int rnd_no=rand()%100;
	for(int i=0;i<n;i++)
		{
			if(rnd_no<w[i])		
				{
					tc.play[0]=(char)(i+48);
					tc.play[1]='\0';
				}
			rnd_no=rnd_no-w[i];
		}
} 

//Function that determines CPU's move :

void cpu_generate()
{
	switch (flag)
		{
				case 0 :	if(strcomp(tpos.tname,tu.tname)==0)
								{
									if(previous[0]=='1' || previous[0]=='2')
									{
										w[0]=0;
										w[1]=30;
										w[2]=30;
										w[3]=20;
										w[4]=20;
										generate(5);
									}
									else
									{
										w[0]=0;
										w[1]=w[2]=w[3]=w[4]=25;
										generate(5);
									}
								}
							else
								{
									w[0]=0;
									w[1]=w[2]=28;
									w[3]=w[4]=22;
									generate(5);
									if(tc.play[0]=='1' || tc.play[1]=='2')
										{
											int i=0;
											i=rand()%15;
											strcat(tc.play," ");
											strcat(tc.play,dribble[i]);
										}
								}
								break;

				case 1	:	if(strcomp(tpos.tname,tu.tname)==0)
								{
									w[0]=w[1]=w[2]=w[3]=w[4]=0;
									w[5]=50;
									w[6]=50;
									generate(7);
								}
							else
								{
									w[0]=0;
									w[1]=w[2]=w[3]=w[4]=5;
									w[5]=40;
									w[6]=40;
									generate(7);
									int i=0;
									i=rand()%15;
									strcat(tc.play," ");
									strcat(tc.play,shot[i]);
								}
								break;

				case 2	:	if(strcomp(tpos.tname,tu.tname)==0)
								{
									w[0]=w[1]=w[2]=w[3]=5;
									w[4]=25;
									w[5]=30;
									w[6]=25;
									generate(7);
								}
							else
								{
									w[0]=1;
									w[1]=w[2]=w[3]=1;
									w[4]=w[5]=w[6]=32;
									generate(7);
								}
								break;

				case 3	:	if(strcomp(tpos.tname,tu.tname)==0)
								{
									w[0]=w[1]=w[2]=w[3]=5;
									w[4]=25;
									w[5]=30;
									w[6]=25;
									generate(7);
								}
							else
								{
									w[0]=1;
									w[1]=w[2]=w[3]=1;
									w[4]=w[5]=w[6]=32;
									generate(7);
									int i=0;
									i=rand()%4;
									strcat(tc.play," ");
									strcat(tc.play,fk[i]);
								}
							break;
		}
								
}
//pass function:

void pass()
{
	flag=0;
	if(strcomp(tpos.tname,tu.tname)==0)
		cout<<"Enter your pass/dribble : ";
	else
		cout<<"Enter your defence : ";
	cin.get();
	gets(tu.play);
	cpu_generate();
	if(((int)(tu.play[0]-48)<0)	||	((int)(tu.play[0]-48)>=5)) 
		{
			if(cscore<27 && cscore>5)
				flag=3;
			else if(cscore>=27 || cscore<=5)
				flag=2;
		}
	strcopy(previous,tu.play);
}

//Function to update the stats :

void update()
{
	switch (flag)
		{
			case 0 :	if(strcomp(tpos.tname,tu.tname)==0)
							{
								if(tu.play[0]==tc.play[0])
									{
										tpos=tc;
										q=0;
									}
								else
									{
										q=1;
										if(tu.side==32)		cscore=cscore-(int)(tu.play[0]-48);
										else 				cscore=cscore+(int)(tu.play[0]-48);
									}
							}
						else
							{
								if(tu.play[0]==tc.play[0])
									{
										q=2;
										tpos=tu;
									}
								else
									{
										q=3;
										if(tu.side==32)		cscore=cscore+(int)(tc.play[0]-48);
										else 				cscore-=(int)(tc.play[0]-48);
									}
							}
						break;

			case 1	:	if(strcomp(tpos.tname,tu.tname)==0)
							{	
								int i=0,counter=1;
								switch (cscore)
									{	
										case 3	:
										case 29 :		for(i=0;i<=1;i++)
															{
																if(strcomp(tu.play+2,shot[i])==0)
																	{
																		counter=0;
																		break;
																	}
															}
														if(counter==1)		
															{
																cout<<"Wrong shot.\nGoal kick..\n";
																cscore=tc.side;
																tpos=tc;
															}
														else
															{
																if(tu.play[0]==tc.play[0])
																	{
																		q=121;
																		cscore=tc.side;
																		tpos=tc;
																	}
																else
																	{
																		q=131;
																		tu.score++;																			
																		cscore=16;
																		tpos=tc;
																	}
															}
														break;

										case 2	:
										case 30 :		for(i=0;i<=3;i++)
															{
																if(strcomp(tu.play+2,shot[i])==0)
																	{
																		counter=0;
																		break;
																	}
															}
														if(counter==1)		
															{
																cout<<"Wrong shot\nGoal kick..\n";
																cscore=tc.side;
																tpos=tc;
															}
														else
															{
																if(tu.play[0]==tc.play[0])
																	{
																		q=122;
																		cscore=tc.side;
																		tpos=tc;
																	}
																else
																	{
																		q=132;
																		tu.score++;		
																		cscore=16;
																		tpos=tc;
																	}
															}
													break;

										case 1	:
										case 31 :		for(i=1;i<=9;i++)
															{
																if(strcomp(tu.play+2,shot[i])==0)
																	{
																		counter=0;
																		break;
																	}
															}
														if(counter==1)		
															{
																cout<<"Wrong shot\nGoal kick..\n";
																cscore=tc.side;
																tpos=tc;
															}
														else
															{
																if(tu.play[0]==tc.play[0])
																	{
																		q=123;
																		cscore=tc.side;
																		tpos=tc;
																	}
																else
																	{
																		q=133;
																		tu.score++;		
																		cscore=16;
																		tpos=tc;
																	}
															}
														break;
										case 0	:
										case 32 :		for(i=1;i<=14;i++)
															{
																if(strcomp(tu.play+2,shot[i])==0)
																	{
																		counter=0;
																		break;
																	}
															}
														if(counter==1)		
															{
																cout<<"Wrong shot\nGoal kick..\n";
																cscore=tc.side;
																tpos=tc;
															}
														else
															{
																if(tu.play[0]==tc.play[0])
																	{
																		q=124;
																		cscore=tc.side;
																		tpos=tc;
																	}
																else
																	{
																		q=134;
																		tu.score++;
																		cscore=16;
																		tpos=tc;
																	}
															}
									}
							}
						else
							{
								if((int)(tu.play[0]-48)!=5 && (int)(tu.play[0]-48)!=6)
									{
										cout<<"Keeper's mistake..\n";
										tc.score++;
										cscore=16;
										tpos=tu;
									}
								else
									{
										int i=0,counter=1;
										switch (cscore)
											{	
												case 3	:
												case 29 :		for(i=0;i<=1;i++)
																	{
																		if(strcomp(tc.play+2,shot[i])==0)
																			{
																				counter=0;
																				break;
																			}
																	}
																if(counter==1)		
																	{
																		cout<<"Wrong shot.\nGoal kick..\n";
																		cscore=tu.side;
																		tpos=tu;
																	}
																else
																	{
																		if(tu.play[0]==tc.play[0])
																			{
																				q=141;
																				cscore=tu.side;
																				tpos=tu;
																			}
																		else
																			{
																				q=151;
																				tc.score++;																			
																				cscore=16;
																				tpos=tu;
																			}
																	}
																break;

												case 2	:
												case 30 :		for(i=0;i<=3;i++)
																	{
																		if(strcomp(tc.play+2,shot[i])==0)
																			{
																				counter=0;
																				break;
																			}
																	}
																if(counter==1)		
																	{
																		cout<<"Wrong shot\nGoal kick..\n";
																		cscore=tu.side;
																		tpos=tu;
																	}
																else
																	{
																		if(tu.play[0]==tc.play[0])
																			{
																				q=142;
																				cscore=tu.side;
																				tpos=tu;
																			}
																		else
																			{
																				q=152;
																				tc.score++;		
																				cscore=16;
																				tpos=tu;
																			}
																	}
															break;

												case 1	:
												case 31 :		for(i=1;i<=9;i++)
																	{
																		if(strcomp(tc.play+2,shot[i])==0)
																			{
																				counter=0;
																				break;
																			}
																	}
																if(counter==1)		
																	{
																		cout<<"Wrong shot\nGoal kick..\n";
																		cscore=tu.side;
																		tpos=tu;
																	}
																else
																	{
																		if(tu.play[0]==tc.play[0])
																			{
																				q=143;
																				cscore=tu.side;
																				tpos=tu;
																			}
																		else
																			{
																				q=153;
																				tc.score++;		
																				cscore=16;
																				tpos=tu;
																			}
																	}
																break;
												case 0	:
												case 32 :		for(i=1;i<=14;i++)
																	{
																		if(strcomp(tc.play+2,shot[i])==0)
																			{
																				counter=0;
																				break;
																			}
																	}
																if(counter==1)		
																	{
																		cout<<"Wrong shot\nGoal kick..\n";
																		cscore=tu.side;
																		tpos=tu;
																	}
																else
																	{
																		if(tu.play[0]==tc.play[0])
																			{
																				q=144;
																				cscore=tu.side;
																				tpos=tu;
																			}
																		else
																			{
																				q=154;
																				tc.score++;
																				cscore=16;
																				tpos=tu;
																			}
																	}
																	break;
 											}
 									}
 							}
						break;

			case 3 :	if(strcomp(tpos.tname,tu.tname)==0)
							{
								int i=0,counter=1;
								for(i=0;i<4;i++)
									{
										if(strcomp(tu.play+2,fk[i])==0)
											{
												counter=0;
												break;
											}
									}
								if((counter==1) || ((int)(tu.play[0]-48)<4 || (int)(tu.play[0]-48)>6))
									{
										cout<<"Bad shot from free kick. You lose possession\n";
										if(tu.side==32)		cscore=cscore-(int)(tu.play[0]-48);
										else 				cscore=cscore+(int)(tu.play[0]-48);
										tpos=tc;
									}
								else
									{

										if(tu.play[0]==tc.play[0])
											{
												q=4;
												tu.score++;
												cscore=16;
												tpos=tc;
											}
										else
											{ 
												q=5;
												cscore=tc.side;
												tpos=tc;
											}
									}
							}
						else
							{
								int i=0,counter=1;
								for(i=0;i<4;i++)
									{
										if(((int)(tc.play[0]-48)<4 || (int)(tc.play[0]-48)>6))
											{
												counter=0;
												break;
											}
									}
								if(counter==1)
									{
										cout<<"Bad shot from free kick from your opponent. You gain possession\n";
										if(tc.side==32)		cscore=cscore-(int)(tc.play[0]-48);
										else 				cscore=cscore+(int)(tc.play[0]-48);
										tpos=tu;
									}
								else
									{
										if(tu.play[0]==tc.play[0])
											{
												q=6;
												tc.score++;
												cscore=16;
												tpos=tu;
											}
										else
											{ 
												q=7;
												cscore=tu.side;
												tpos=tu;
											}
									}
							}
	      				break;

	      	case 2 :	if(strcomp(tpos.tname,tu.tname)==0)
							{
								if((int)(tu.play[0]-48)<4 || (int)(tu.play[0]-48)>6)
									{
										cout<<"The ball went sailing into the sky. Goal Kick..!!\n";
										tpos=tc;
										cscore=tc.side;
									}
								else
									{
										if(tu.play[0]==tc.play[0])
											{
												q=8;
												tpos=tc;
												cscore=tc.side;
											}
										else
											{
												q=9;
												tu.score++;
												cscore=16;
												tpos=tc;
											}
									}
							}
						else
							{
								if((int)(tc.play[0]-48)<4 || (int)(tc.play[0]-48)>6)
									{
										cout<<"The ball went sailing into the sky. Goal Kick..!!\n";
										tpos=tu;
										cscore=tu.side;
									}
								else
									{
										if(tu.play[0]==tc.play[0])
											{
												q=10;
												tpos=tu;
												cscore=tu.side;
											}
										else
											{
												q=11;
												tc.score++;
												cscore=16;
												tpos=tu;
											}
									}
							}
						break;
		}
}

//Function to comment about passing or dribbling :

void comment_pass()
{
	switch(q)
		{
			case 0	:	cout<<"You are dispossessed. Practise more, bruh?\n";
						break;
			case 1	:	switch(tu.play[0])
							{
								case '4'	:	cout<<"That long pass switched the play.\n";
												break;
								case '3'	:	cout<<"A perfectly timed pass worthy of envy of Paul Scholes \n";
												break;
								case '1'	:
								case '2'	:	for(int i=0;i<=14;i++)
													{
														if(strcomp(tu.play+2,dribble[i])==0)
															cout<<comm_dribble[i];
													}
												break;	
							}
						break;
			case 2	:	cout<<"Opponent dispossessed. That tackle was Maldini-ishly beautiful.\n";
						break;
			case 3	:	switch(tc.play[0])
							{
								case '4'	:	cout<<"That long pass switched the play.\n";
												break;
								case '3'	:	cout<<"A perfectly timed pass worthy of envy of Paul Scholes \n";
												break;
								case '1'	:
								case '2'	:	for(int i=0;i<=14;i++)
													{
														if(strcomp(tc.play+2,dribble[i])==0)
															cout<<comm_dribble[i];
													}
													break;	
							}
						break;
		}
}

//Function to comment about shooting :

void comment_shoot()
{
	switch(q)
		{	case 141	:
			case 121	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"That long ranger was ruthlessly stopped by the keeper..!! Wonderful..!!\n ";
							cout<<"A goal kick to be taken by the keeper.\n";
							break;

			case 142	:
			case 122	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"The shot was perfectly placed and paced..But still the goalkeeper had a other ideas..!!\n ";
							cout<<"Its a brilliant save.";
							cout<<"A goal kick to be taken by the keeper.\n";
							break;

			case 143	:	
			case 123	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"And it's a save..!! This man is playing the match of his life, perhaps.\n";
							cout<<"A goal kick to be taken by the keeper.\n";
							break;

			case 144	:	
			case 124	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"It's a save..!! Did he inherit those reflexes from God??\n";
							cout<<"A goal kick to be taken by the keeper.\n";
							break;

			case 151	:	
			case 131	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"That rocket burned through the goal net to satisfy itself..\n";
							cout<<"What a magnificient shot..!!\n";
							cout<<"The opponent to take the center.";
							break;

			case 152	:	
			case 132	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"Its's a screamer..!! Just perfectly placed, beyond the goalkeeper's reach \n";
							cout<<"Beautiful..\n";
							cout<<"The Opponent to take the center.";
							break;

			case 153	:
			case 133	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"And its all the wayyy..!! Wonderful,Wonderful, Wonderful..!! How good is he..?? \n";
							cout<<"Beautiful..\n";
							cout<<"The Opponent to take the center.";
							break;

			case 154	:	
			case 134	:	for(int i=0;i<=14;i++)
								{
									if(strcomp(tpos.tname,tu.tname)==0)
									{
										if(strcomp(tu.play+2,shot[i])==0)
											cout<<comm_shot[i];
									}
									else
										if(strcomp(tc.play+2,shot[i])==0)
											cout<<comm_shot[i];
								}
							usleep(1000000);
							cout<<"That's a world class finish..!! I think he has ice flowing through his veins..!! \n";
							cout<<"Brutally Clinical and perfect..\n";
							cout<<"The Opponent to take the center.";
							break;


		}
}



//Function to comment about penalty :

void comment_penalty()
{
	switch(q)
		{
			case 10	:
			case 8	:	cout<<"And he saved it..!! Pitch perfect reflexes..!! Beastly instincts..!\n ";
						cout<<"Oh man..!! I love him..\n";
						cout<<"The keeper to take a goal kick.\n";
						break;

			case 11	:
			case 9	:	cout<<"And he scores..!! He never looked like missing it.\n";
						cout<<"Well I think he got nerves of steel..The way he took it, the look, the shot, everything..\n";
						cout<<"The Opponent to take the center.\n";
						break;
		}
}

//Function to comment about Free Kicks 	:

void comment_freekick()
{
	switch(q)
		{
			case 4	:
			case 6	:	for(int i=0;i<4;i++)
							{
								if(strcomp(tpos.tname,tu.tname)==0)
									{

										if(strcomp(tu.play+2,fk[i])==0)
											cout<<comm_fk[i];	
									}
								else
									{
										if(strcomp(tc.play+2,fk[i])==0)
											cout<<comm_fk[i];
									}
							}
						usleep(1000000);
						cout<<"And he scores..!! Oh yes, oh yes..!! Did you doubt him??\n";
						cout<<"He is back on the score sheet and look how..!!\n";
						cout<<"The Opponent to start with a center.";
						break;

			case 5	:
			case 7	:	for(int i=0;i<4;i++)
							{
								if(strcomp(tpos.tname,tu.tname)==0)
									{

										if(strcomp(tu.play+2,fk[i])==0)
											cout<<comm_fk[i];	
									}
								else
									{
										if(strcomp(tc.play+2,fk[i])==0)
											cout<<comm_fk[i];
									}
							}
						usleep(1000000);
						cout<<"Ohhh...It was close, but still, the goalkeeper can be tested better.\n";
						cout<<"The ball was almost perfectly placed, making it very difficult for the keeper to kepp it out.\n";
						cout<<"The Keeper to take a goal kick.\n";
						break;
		}
}

//Function to display relevant comments :

void comment()
{
	switch(flag)
		{
			case 0	:	comment_pass();
						break;
			case 1	:	comment_shoot();
						break;
			case 2	:	comment_penalty();
						break;
			case 3	:	comment_freekick();
						break;
		}
}
											

//Function for shooting moves like normal goal ahot, freekick or penalty:

void shoot()
{
	cout<<"Enter your shot/defence : ";
	cin.get();
	gets(tu.play);
	cpu_generate();
	system("clear");
	update();
	display();
	comment();
}

int main()
{
	srand(time(0));

	strcopy(dribble[0],"s");
	strcopy(dribble[1],"4");
	strcopy(dribble[2],"6");
	strcopy(dribble[3],"464");
	strcopy(dribble[4],"646");
	strcopy(dribble[5],"446");
	strcopy(dribble[6],"664");
	strcopy(dribble[7],"82");
	strcopy(dribble[7],"282");
	strcopy(dribble[8],"2486");
	strcopy(dribble[9],"2684");
	strcopy(dribble[10],"246");
	strcopy(dribble[11],"264");
	strcopy(dribble[12],"24426");
	strcopy(dribble[13],"882");
	strcopy(dribble[14],"84");

	strcopy(comm_dribble[0],"The player played a short pass to his team mate, before moving forward to join the attack.\n");
	strcopy(comm_dribble[1],"He dribbles past the defender leftward with his feint. Umm..Remember that Argentine, No. 10??\n");
	strcopy(comm_dribble[2],"He dribbles past the defender rightward with his feint. Umm..Remember that Argentine, No. 10??\n");
	strcopy(comm_dribble[3],"Ohh..That's a magnificent step over, moving towards the left..Bro, is he a student of Eric Cantona ??\n");
	strcopy(comm_dribble[4],"Ohh..That's a magnificent step over, moving towards the right..Bro, is he a student of Eric Cantona ??\n");
	strcopy(comm_dribble[5],"Ohh..The player cuts through the defence with his perfect chop.Is he that Portuguese No. 7??\n");
	strcopy(comm_dribble[6],"Ohh..The player cuts through the defence with his perfect chop.Is he that Portuguese No. 7??\n");
	strcopy(comm_dribble[7],"He flicked with his heel, nutmegging the player. This lad has got some serious style.\n");
	strcopy(comm_dribble[8],"Ohh..!! That roulette completely destroyed the defender's move. He surges to the left.\n ");
	strcopy(comm_dribble[9],"Ohh..!! That roulette completely destroyed the defender's move. He surges to the right.\n ");
	strcopy(comm_dribble[10],"Ohhh God..!! He \"elastically\" tries to crush the defender, and moves to the right..\n");
	strcopy(comm_dribble[11],"Ohhh God..!! He \"elastically\" tries to crush the defender, and moves to the left..\n");
	strcopy(comm_dribble[12],"That Hocus Pocus just triggered a cyclone in my stomach..!! Bro, how did he do that??\n ");
	strcopy(comm_dribble[13],"He soberly delivers that Sombrero, . Perfect..\n");
	strcopy(comm_dribble[14],"Oh my my..!! Did I just see a Berba spin at its best ??\n");

	strcopy(shot[0],"k");
	strcopy(shot[1],"p");
	strcopy(shot[2],"v");
	strcopy(shot[3],"f");
	strcopy(shot[4],"m");
	strcopy(shot[5],"c");
	strcopy(shot[6],"bc");
	strcopy(shot[7],"bv");
	strcopy(shot[8],"scr");
	strcopy(shot[9],"t");
	strcopy(shot[10],"sc");
	strcopy(shot[11],"r");
	strcopy(shot[12],"fl");
	strcopy(shot[13],"bh");
	strcopy(shot[14],"to");

	strcopy(comm_shot[0],"That knuckleball shot ensured that only GOD knows which way it will turn.\n");
	strcopy(comm_shot[1],"That's a pretty powerful shot, worthy of competing with a bullet.\n");
	strcopy(comm_shot[2],"That was a perfectly timmed volley..Beautiful..!!\n");
	strcopy(comm_shot[3],"Ohh man...!! It perfectly curled its way in the air..The Picasso of football is right here.\n");
	strcopy(comm_shot[4],"That was a medium paced shot, but its accuracy is sure to test the goalkeeper..\n");
	strcopy(comm_shot[5],"Ohh..!! That's a wonderful chip shot, right with the head of the boot.\n");
	strcopy(comm_shot[6],"And its a bicycle kick..!! Man, what the hell did I just see ??\n");
	strcopy(comm_shot[7],"That back volley was completely unexpected, catching everyone by surprise..!!\n");
	strcopy(comm_shot[8],"Oh goodness me..!! That scissor kick reminded me of someone..The King of Sweden..No. 10??\n");
	strcopy(comm_shot[9],"That tap-in was clinical,a and is sure to test the reflexes of the keeper.\n");
	strcopy(comm_shot[10],"Oh my God..!! That scissor kick reminded me of someone..The King of Sweden..No. 10??\n");
	strcopy(comm_shot[11],"That was a perfect Rabona..!! Its still a mystery to me how he did that so effortlessly..\n");
	strcopy(comm_shot[12],"He effortlessly flicked the ball past him..It has come wonderfully out of him..\n");
	strcopy(comm_shot[13],"Ohh..Everyone out there on the pitch applauded him for that backheel.Well done..!!\n");
	strcopy(comm_shot[14],"He kicked it with his toe..The ball quite elastically came off his shoe..Brilliant.\n");

	
	strcopy(fk[0],"c");
	strcopy(fk[1],"k");
	strcopy(fk[2],"d");
	strcopy(fk[3],"p");

	strcopy(comm_fk[0],"He curled the ball beautifully, as the ball kissed the air and glided towards the goal.\n");
	strcopy(comm_fk[1],"Oh GOD..The ball twisted and turned in different directions, playing with the keeper's eyesight\n");
	strcopy(comm_fk[2],"He triedto dip the ball into the net. He might make it through,can he??\n");
	strcopy(comm_fk[3],"Oh man..!! Can he stop that powerful shot disguised as a rocket??\n");

	strcopy(previous,"NULL");

	cout<<"\tWelcome to "<<endl;
	cout<<"Enter Your team name : ";
	gets(tu.tname);
	cout<<"Enter CPU's team name : ";

	gets(tc.tname);
	cout<<"Enter Maximum number of goals to be scored by a team for winning : ";
	cin>>maximum;
	cout<<"The two captains shake hands with the referees, and are clearly geared up for the toss.\n";
	usleep(1000000);
	toss_func();
	
	char resp;												//Response regarding whether he would want to play or not
	cout<<"Press c to begin, or q to quit the game \n";
	cin>>resp;
	if(resp=='q')	return 0;
	else
	cout<<"Let the match begin !!\n";
	display();
	cout<<"\nThe referee whistles, signalling the player to start.\n";
	while(tu.score<maximum && tc.score<maximum)
	{
		pass();
		system("clear");
		update();
		display();
		comment();
		if(flag==2)
			{
				cout<<"That's a clear foul..!! No mercy. It's a penalty\n";
				usleep(1000000);
				cout<<"Does he have the nerves of steel or is it his looks winning him a place in the team? \n";
				shoot();
			}
		else if(flag==3)
			{
				cout<<"That is a ruthless tackle.. Its a free kick..!!\n";
				usleep(1000000);
				cout<<"Deep Breaths,eagle focus. What is he gonna do ?\n";
				shoot();
			}
		if(cscore>=29 || cscore<=3)
			{
				flag=1;
				if(strcomp(tpos.tname,tu.tname)==0)
					cout<<"You have a chance to score..!!\n";
				else
					cout<<"Your opponent has a chance to score !! \n";
				shoot();

			}
		 
		
	}
	if(tu.score==maximum)	cout<<tu.tname<<"beats "<<tc.tname<<"with the final score of "<<tu.score<<" - "<<tc.score<<endl;
	else	cout<<tc.tname<<"beats "<<tu.tname<<"with the final score of "<<tu.score<<" - "<<tc.score<<endl;
	cout<<"Thanks for enjoying the match. Its been s great day of football, having all the thrills and frills a fan can expect.\n";
	cout<<"Thank you..!! Have a nice day.!!\n";
	return 0;
	
}