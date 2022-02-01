; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Char: %c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %test = alloca i8*, align 8
  %0 = alloca i8*, align 8
  %c1 = alloca i8, align 1
  %1 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  %2 = load i8*, i8** %0, align 8
  store i8* %2, i8** %test, align 8
  %3 = load i8*, i8** %test, align 8
  store i32 2, i32* %1, align 4
  %4 = load i32, i32* %1, align 4
  %5 = getelementptr inbounds i8, i8* %3, i32 %4
  %6 = load i8, i8* %5, align 1
  store i8 %6, i8* %c1, align 1
  %7 = load i8, i8* %c1, align 1
  %8 = zext i8 %7 to i32
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0), i32 %8)
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}
