; ModuleID = 'source.spice'
source_filename = "source.spice"

%Letter = type { i8* }

@0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Content: %s\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i8* @"getContent()"(%Letter* %0) #0 {
entry:
  %this = alloca %Letter*, align 8
  store %Letter* %0, %Letter** %this, align 8
  %result = alloca i8*, align 8
  %1 = getelementptr inbounds %Letter*, %Letter** %this, i32 0
  %2 = load %Letter*, %Letter** %1, align 8
  %3 = getelementptr inbounds %Letter, %Letter* %2, i32 0, i32 0
  %4 = load i8*, i8** %3, align 8
  ret i8* %4
}

; Function Attrs: nounwind
define void @"setContent(string)"(%Letter* %0, i8* %1) #0 {
entry:
  %this = alloca %Letter*, align 8
  store %Letter* %0, %Letter** %this, align 8
  %text = alloca i8*, align 8
  store i8* %1, i8** %text, align 8
  %2 = getelementptr inbounds i8*, i8** %text, i32 0
  %3 = load i8*, i8** %2, align 8
  %4 = getelementptr inbounds %Letter*, %Letter** %this, i32 0
  %5 = load %Letter*, %Letter** %4, align 8
  %6 = getelementptr inbounds %Letter, %Letter* %5, i32 0, i32 0
  store i8* %3, i8** %6, align 8
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %letter = alloca %Letter, align 8
  %0 = alloca i8*, align 8
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  %1 = load i8*, i8** %0, align 8
  %2 = getelementptr inbounds %Letter, %Letter* %letter, i32 0, i32 0
  store i8* %1, i8** %2, align 8
  %3 = alloca i8*, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @1, i32 0, i32 0), i8** %3, align 8
  %4 = load i8*, i8** %3, align 8
  call void @"setContent(string)"(%Letter* %letter, i8* %4)
  %5 = alloca i1, align 1
  store i1 true, i1* %5, align 1
  %6 = getelementptr inbounds i1, i1* %5, i32 0
  %7 = call i8* @"getContent()"(%Letter* %letter)
  %8 = alloca i8*, align 8
  store i8* %7, i8** %8, align 8
  %9 = getelementptr inbounds i8*, i8** %8, i32 0
  %10 = load i8*, i8** %9, align 8
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i8* %10)
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

attributes #0 = { nounwind }
