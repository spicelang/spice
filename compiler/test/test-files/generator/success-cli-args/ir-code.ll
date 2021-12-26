; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [9 x i8] c"Argc: %d\00", align 1
@1 = private unnamed_addr constant [15 x i8] c"Argv no. 0: %s\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"Argv no. 1: %s\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %argc = alloca i32, align 4
  store i32 %0, i32* %argc, align 4
  %argv = alloca i8**, align 8
  store i8** %1, i8*** %argv, align 8
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %2 = getelementptr inbounds i32, i32* %argc, i32 0
  %3 = load i32, i32* %2, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0), i32 %3)
  %5 = alloca i32, align 4
  store i32 0, i32* %5, align 4
  %6 = load i32, i32* %5, align 4
  %7 = getelementptr inbounds i8**, i8*** %argv, i32 %6
  %8 = load i8**, i8*** %7, align 8
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @1, i32 0, i32 0), i8** %8)
  %10 = getelementptr inbounds i32, i32* %argc, i32 0
  %11 = alloca i32, align 4
  store i32 1, i32* %11, align 4
  %12 = load i32, i32* %10, align 4
  %13 = load i32, i32* %11, align 4
  %gt = icmp sgt i32 %12, %13
  %14 = alloca i1, align 1
  store i1 %gt, i1* %14, align 1
  %15 = load i1, i1* %14, align 1
  br i1 %15, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %16 = alloca i32, align 4
  store i32 1, i32* %16, align 4
  %17 = load i32, i32* %16, align 4
  %18 = getelementptr inbounds i8**, i8*** %argv, i32 %17
  %19 = load i8**, i8*** %18, align 8
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @2, i32 0, i32 0), i8** %19)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}
