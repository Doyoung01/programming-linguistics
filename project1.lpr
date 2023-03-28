program project1;

uses
 Sysutils;

const
  C_FNAME = 'students.txt';

type
  tuple = record
            str: string;
            int: integer;
  end;

  info = record
            ID: string;
            name: string;
            scores: packed array[0..10] of tuple;
            average: real;
          end;
var
  tfOut, tfIN: TextFile;
  str: string;
  i, idx, tidx, sum, count: integer;
  new: info;
  t: tuple;

  stuarr: array[0..10] of info;

  tmp: TStringArray;

procedure quickSortArray(var inputArray : array of info; lowPos, highPos : integer);
var
  movablePointer, immovablePointer, temporaryPointer : integer;
  temporaryItem : info;
begin
  immovablePointer := lowPos;
  movablePointer := highPos;

  while (movablePointer <> immovablePointer) do
  begin
    if(movablePointer > immovablePointer) then
    begin
      if(inputArray[movablePointer].average < inputArray[immovablePointer].average) then begin
        temporaryItem := inputArray[movablePointer];
        inputArray[movablePointer] := inputArray[immovablePointer];
        inputArray[immovablePointer] := temporaryItem;

        temporaryPointer := movablePointer;
        movablePointer := immovablePointer;
        immovablePointer := temporaryPointer;
      end
      else
      begin
        dec(movablePointer);
      end;
    end
    else
    begin
      if(inputArray[movablePointer].average > inputArray[immovablePointer].average) then
      begin
        temporaryItem := inputArray[movablePointer];
        inputArray[movablePointer] := inputArray[immovablePointer];
        inputArray[immovablePointer] := temporaryItem;

        temporaryPointer := movablePointer;
        movablePointer := immovablePointer;
        immovablePointer := temporaryPointer;
      end
      else
      begin
        inc(movablePointer);
      end;
    end;
  end;

  if(movablePointer > lowPos) then
    quickSortArray(inputArray,lowPos,movablePointer-1);

  if(movablePointer < highPos) then
    quickSortArray(inputArray,movablePointer+1,highPos);
end;

procedure printOut(stuarr: array of info; a, b: integer);
var
  i, j: integer;
begin
  if a < b then begin
    for i := 0 to 10 do begin
      if stuarr[i].name = '' then break;
      writeln('Name: ', stuarr[i].name, ' ID: ', stuarr[i].ID);
      writeln('Average: ', stuarr[i].average:3:2);
      write('(subject, score): ');
      for j := 0 to 10 do begin
        if stuarr[i].scores[j].str = '' then break;
        write('(', stuarr[i].scores[j].str, ', ', stuarr[i].scores[j].int, ') ');
      end;
      writeln(); writeln();
    end;
  end
  else begin
    for i := 10 downto 0 do begin
      if stuarr[i].name = '' then break;
      writeln('Name: ', stuarr[i].name, ' ID: ', stuarr[i].ID);
      writeln('Average: ', stuarr[i].average:3:2);
      write('(subject, score): ');
      for j := 0 to 10 do begin
        if stuarr[i].scores[j].str = '' then break;
        write('(', stuarr[i].scores[j].str, ', ', stuarr[i].scores[j].int, ') ');
      end;
      writeln(); writeln();
    end;
  end;
end;

begin
  { 학생들의 정보 입력 받기 }
  str := '';
  writeln('Please write down the students list at stduents.txt file.');

  AssignFile(tfOut, C_FNAME);
  {$I+}
  try
    rewrite(tfOut);

    writeln('If you want to stop writing, type "stop".');
     while true do begin
       write('ID of the student : ');
       readln(str);
       if str = 'stop' then break;
       writeln(tfOut, str);
       write('Name of the student : ');
       readln(str);
       writeln(tfOut, str);
       write('Score of the student : ');
       readln(str);
       writeln(tfOut, str);
       writeln();
     end;

    CloseFile(tfOut);

  except
    on E: EInOutError do
      writeln('File handling error occurred. Details: ', E.ClassName, '/', E.Message);
  end;

  writeln('File ', C_FNAME, ' created if all went ok.');
  writeln();


  { students.txt 파일에서 Record 요소(특히 점수 부분)를 나눈 후 저장하기 }
  AssignFile(tfIn, C_FNAME);
  try
    reset(tfIn); { 저장된 학생 리스트를 읽기 모드로 open }
    count := 0; i := 0; { 학생의 수 }
    while not eof(tfIn) do
      begin
        readln(tfIn, str);
        if (i mod 3 = 0) then begin new.ID := str; i += 1; end
        else if (i mod 3 = 1) then begin new.name:= str; i += 1; end
        else if (i mod 3 = 2) then begin
          tmp := str.Split(' ');
          tidx := 0; idx := 0; sum := 0;
          while (tmp[tidx] <> '') do begin
            t.str := tmp[tidx]; tidx += 1;
            t.int := StrToInt(tmp[tidx]); tidx += 1;
            sum += t.int;
            new.scores[idx] := t; idx += 1;
          end;
          new.average := sum div idx;
          i += 1;
          stuarr[count] := new;
          count += 1;
        end;
      end;

    CloseFile(tfIn);
  except
    on E: EInOutError do
     writeln('File handling error occurred. Details: ', E.Message);
  end;

  { 입력된 학생들의 정보를 출력하기 }
  writeln('This is the list of students you entered.');
  writeln('--------------------------------------------------------------------');
  printOut(stuarr, 0, 10);
  writeln('--------------------------------------------------------------------');
  writeln('This is the file ', C_FNAME, ' you typed. Press enter to go next step.');
  readln;


  { average로 정렬하기 }
  quickSortArray(stuarr, 0, 10);

  writeln('--------------------------------------------------------------------');
  writeln('<Ordered List>');
  writeln('--------------------------------------------------------------------');
  { 정렬된 list 출력 }
  printOut(stuarr, 10, 0);
  writeln('--------------------------------------------------------------------');


  writeln('The file ', C_FNAME, ' was ordered. Press enter to stop.');
  readln;
end.
