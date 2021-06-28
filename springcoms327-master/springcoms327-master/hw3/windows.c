#include <stdio.h>
#include <stdlib.h>

struct window{
	unsigned xlow, ylow, xhigh, yhigh;
	char wch;
};

int dxlow, dylow, dxhigh, dyhigh;
FILE* fp;

void fill_line(struct window W[], unsigned Wsize, unsigned x1, unsigned x2, unsigned y, char line[]);
unsigned fill_windows(struct window W[], unsigned wmax);

void fill_line(struct window W[], unsigned Wsize, unsigned x1, unsigned x2, unsigned y, char line[]){
	int count=0;
	while(count<Wsize){
		if(W[count].ylow<=y && W[count].yhigh>=y){
			int x=W[count].xlow;
			while(x<=W[count].xhigh){
				line[x]=W[count].wch;
				x++;
			}
		}
		count++;
	}
}

unsigned fill_windows(struct window W[], unsigned wmax){
	//reads the coordinates of the screen to display
	fscanf(fp, "%u%*c%u %u%*c%u", &dxlow, &dylow, &dxhigh, &dyhigh);
	// result[dyhigh][dxhigh];
	unsigned numw;
	//reads total number of windows
	fscanf(fp, "%u", &numw);
	int count=0;
	while(count<numw){
		// read format xlow,ylow xhigh,yhigh ch
		fscanf(fp, "%u%*c%u %u%*c%u %c", &W[count].xlow, &W[count].ylow, &W[count].xhigh, &W[count].yhigh, &W[count].wch);
		count++;
	}
	return numw;
}

int main(int argc, char *argv[]){
	if(argc==2){
		fp=fopen(argv[1], "r");
	}
	else{
		fp=stdin;
	}
	unsigned wmax=100;
	struct window w[100];
	//reads input from file
	int numw=fill_windows(w, wmax);
	int count=0;
	while(count<numw){
		if(w[count].xhigh > dxhigh){
			w[count].xhigh=dxhigh;
		}
		if(w[count].yhigh > dyhigh){
			w[count].yhigh=dyhigh;
		}
		if(w[count].xlow < dxlow){
			w[count].xlow=dxlow;
		}
		if(w[count].ylow < dylow){
			w[count].ylow=dylow;
		}
		count++;
	}

	for(int i=dylow; i<=dyhigh; i++){
		int length=dxhigh-dxlow+1;
		char line[length];
		for(int i=0; i<=length; i++){
			line[i]=' ';
		}
		fill_line(w, numw, dxlow, dxhigh, i, line);
		int j=sizeof(line);
		line[j+1]='\0';
		printf("%s\n", line);
	}

	fclose(fp);
	return 0;
}
