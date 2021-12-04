; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [15 x i8] c"Condition true\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Condition false\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 3, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 3, i32* %1, align 4
  %2 = load i32, i32* %0, align 4
  %3 = load i32, i32* %1, align 4
  %eq = icmp eq i32 %2, %3
  %4 = alloca i1, align 1
  store i1 %eq, i1* %4, align 1
  %5 = load i1, i1* %4, align 1
  %condition = alloca i1, align 1
  store i1 %5, i1* %condition, align 1
  %6 = getelementptr inbounds i1, i1* %condition, i32 0
  %7 = load i1, i1* %6, align 1
  br i1 %7, label %then, label %end

then:                                             ; preds = %entry
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0))
  %9 = alloca i32, align 4
  store i32 0, i32* %9, align 4
  %10 = load i32, i32* %9, align 4
  ret i32 %10

end:                                              ; preds = %entry
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @1, i32 0, i32 0))
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}
