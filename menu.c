#include "g_local.h"


void addlinetomenu(edict_t *ent,char *line,int option)
{

if (ent->client->menustorage.menu_active) // checks to see if the menu is showing
	return;
if (ent->client->menustorage.num_of_lines>12) // checks to see if there is space
	return;


ent->client->menustorage.num_of_lines++; // adds to the number of lines that can be seen

ent->client->menustorage.messages[ent->client->menustorage.num_of_lines].msg = malloc(22); // allocates memory for the string
//sprintf(ent->client->menustorage.messages[ent->client->menustorage.num_of_lines].msg,"%-21s",line); // copies the first 21 characters to the storage string. the 22nd character is for the delimiting '/0'
sprintf_s(ent->client->menustorage.messages[ent->client->menustorage.num_of_lines].msg, 22, "%s", line);
ent->client->menustorage.messages[ent->client->menustorage.num_of_lines].option=option;
}


void clearmenu(edict_t *ent)
{
int i=0;


if (ent->client->menustorage.menu_active) // checks to see if the menu is showing
	return;


for (i=0;i<12;i++){
ent->client->menustorage.messages[i].option=0;
if (ent->client->menustorage.messages[i].msg!=NULL)
	free(ent->client->menustorage.messages[i].msg);
}

ent->client->menustorage.optionselected=NULL;
ent->client->menustorage.currentline=0;
ent->client->menustorage.num_of_lines=0;
}






void setmenuhandler(edict_t *ent, void (*optionselected)(edict_t *ent,int option))
{
ent->client->menustorage.optionselected=optionselected;
}





void menudown(edict_t *ent)
{
int 	i;

i=1;


do
{
if (ent->client->menustorage.currentline<ent->client->menustorage.num_of_lines)
	ent->client->menustorage.currentline++;
else
{
while ((ent->client->menustorage.messages[i].option==0)&(i<ent->client->menustorage.num_of_lines))
	{i++;}
ent->client->menustorage.currentline=i;
}
}
while (ent->client->menustorage.messages[ent->client->menustorage.currentline].option==0);
showmenu(ent);
}





void menuup(edict_t *ent)
{
int 	i;



i=ent->client->menustorage.num_of_lines;
do
{
if (ent->client->menustorage.currentline>2)
ent->client->menustorage.currentline--;
else
{
while ((ent->client->menustorage.messages[i].option==0)|i<ent->client->menustorage.num_of_lines)
	{i--;}
ent->client->menustorage.currentline=i;
}
}
while (ent->client->menustorage.messages[ent->client->menustorage.currentline].option==0);

showmenu(ent);
}



void menuselect(edict_t *ent)
{
int i;

i=ent->client->menustorage.messages[ent->client->menustorage.currentline].option;
closemenu(ent); // close the menu as a selection has been made
ent->client->menustorage.optionselected(ent,i); // call the select function with the value that has been alocated for that selection.
ent->client->menustorage.optionselected=NULL;
}

/*

Explicitly set all the variables for each client to null values

*/


void initmenu(edict_t *ent)
{
int i;

for (i=0;i<12;i++)
{
	ent->client->menustorage.messages[i].msg=NULL;
	ent->client->menustorage.messages[i].option=0;
}

ent->client->menustorage.menu_active=false;
ent->client->menustorage.displaymsg;
ent->client->menustorage.optionselected=NULL;
ent->client->menustorage.currentline=0;
ent->client->menustorage.num_of_lines=0;

}


/*
used every frame to display the menu.
*/


void showmenu(edict_t *ent)
{

int i,j;  // general purpose integer for temporary use :)
char finalmenu[1024]; // where the control strings for the menu are assembled.
char tmp[80],tmp2[80]; // temporary storage strings



sprintf (finalmenu, "xv 32 yv 8 picn inventory ");  // displays the inventory picture (well, its the control codes for it. this doesn't actually display it yet)

j=48; // y coord for text

for (i=1;i<(ent->client->menustorage.num_of_lines+1);i++)    // to print all the lines. options in green with a marker next to the selected one. all other text in white.
{
	gi.cprintf(ent,PRINT_HIGH,"%1 \n",ent->client->menustorage.messages[i].option);
	if (ent->client->menustorage.messages[i].option==0)
	{
		sprintf(tmp,"xv 52 yv %i string \" %s \" ",j,ent->client->menustorage.messages[i].msg);
	}
	else if (i==ent->client->menustorage.currentline)
	{
		sprintf(tmp,"xv 52 yv %i string2 \">>  %s <<\" ",j,ent->client->menustorage.messages[i].msg);
	}
	else 
		sprintf(tmp,"xv 52 yv %i string2 \"   %s   \" ",j,ent->client->menustorage.messages[i].msg); 

	strcat(finalmenu,tmp);
	j+=12;
}

	ent->client->menustorage.menu_active = true;

	ent->client->menustorage.displaymsg = false;
	ent->client->showinventory = false;
	ent->client->showscores = true;
	gi.WriteByte (svc_layout);
	gi.WriteString (finalmenu);
	gi.unicast (ent, true);
}


void closemenu(edict_t *ent)
{

clearmenu(ent);

ent->client->showscores = false;
ent->client->menustorage.menu_active = false;
ent->client->menustorage.displaymsg = false;
ent->client->showinventory = false;


}

void testmenuhandler(edict_t *ent,int option)
{
switch (option)
{
case 1:
gi.centerprintf(ent,"option1\n");
break;
case 2:
gi.centerprintf(ent,"option2\n");
break;
case 3:
gi.centerprintf(ent,"option3\n");
break;
default:
gi.centerprintf(ent,"fuckup\n");
}
}





void Menu_test(edict_t *ent)
{

   if (ent->client->showscores || ent->client->showinventory || ent->client->menustorage.menu_active)
        return;



clearmenu(ent);
addlinetomenu(ent,"This is a test menu",0);

addlinetomenu(ent,"option 1",1);
addlinetomenu(ent,"option 2",2);
addlinetomenu(ent,"misc text",0);
addlinetomenu(ent,"option 3",3);
setmenuhandler(ent,testmenuhandler);
ent->client->menustorage.currentline=2;
showmenu(ent);

}

void clearallmenus(edict_t *ent) // this is just here to help with encapsulation of code to this file
{
	int i;

	for (i=0 ; i<game.maxclients ; i++)
	{
		ent = g_edicts + 1 + i;
		closemenu(ent);
	}
}
