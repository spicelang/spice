; ModuleID = 'source.spice'
source_filename = "source.spice"

%Letter = type { i8* }

@0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i8* @"getContent()"(%Letter* %0) #0 {
entry:
  %this = alloca %Letter*, align 8
  %result = alloca i8*, align 8
  store %Letter* %0, %Letter** %this, align 8
  %1 = load %Letter*, %Letter** %this, align 8
  %2 = load %Letter*, %Letter** %this, align 8
  %3 = getelementptr %Letter, %Letter* %2, i32 0, i32 0
  %4 = load i8*, i8** %3, align 8
  ret i8* %4
}

; Function Attrs: nounwind
define void @"setContent(string)"(%Letter* %0, i8* %1) #0 {
entry:
  %this = alloca %Letter*, align 8
  %text = alloca i8*, align 8
  store %Letter* %0, %Letter** %this, align 8
  store i8* %1, i8** %text, align 8
  %2 = load i8*, i8** %text, align 8
  %3 = load %Letter*, %Letter** %this, align 8
  %4 = load %Letter*, %Letter** %this, align 8
  %5 = getelementptr %Letter, %Letter* %4, i32 0, i32 0
  store i8* %2, i8** %5, align 8
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %Letter = alloca %Letter, align 8
  %0 = alloca i8*, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i1, align 1
  %3 = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  %4 = load i8*, i8** %0, align 8
  %5 = getelementptr inbounds %Letter, %Letter* %Letter, i32 0, i32 0
  store i8* %4, i8** %5, align 8
  %6 = load %Letter, %Letter* %Letter, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @1, i32 0, i32 0), i8** %1, align 8
  %7 = load i8*, i8** %1, align 8
  call void @"setContent(string)"(%Letter* %Letter, i8* %7)
  store i1 true, i1* %2, align 1
  %8 = load %Letter, %Letter* %Letter, align 8
  %9 = call i8* @"getContent()"(%Letter* %Letter)
  store i8* %9, i8** %3, align 8
  %10 = load i8*, i8** %3, align 8
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @2, i32 0, i32 0), i8* %10)
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

attributes #0 = { nounwind }
