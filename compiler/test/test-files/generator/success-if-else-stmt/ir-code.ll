; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 1
@2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"Else\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 1, i32* %1, align 4
  %2 = load i32, i32* %0, align 4
  %3 = load i32, i32* %1, align 4
  %ne = icmp ne i32 %2, %3
  %4 = alloca i1, align 1
  store i1 %ne, i1* %4, align 1
  %5 = load i1, i1* %4, align 1
  br i1 %5, label %then, label %else

then:                                             ; preds = %entry
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0))
  br label %end7

else:                                             ; preds = %entry
  %7 = alloca double, align 8
  store double 2.000000e+00, double* %7, align 8
  %8 = alloca double, align 8
  store double 3.141500e+00, double* %8, align 8
  %9 = load double, double* %7, align 8
  %10 = load double, double* %8, align 8
  %eq = fcmp oeq double %9, %10
  %11 = alloca i1, align 1
  store i1 %eq, i1* %11, align 1
  %12 = load i1, i1* %11, align 1
  br i1 %12, label %then1, label %else2

then1:                                            ; preds = %else
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0))
  br label %end6

else2:                                            ; preds = %else
  %14 = alloca double, align 8
  store double 2.000000e+00, double* %14, align 8
  %15 = alloca double, align 8
  store double 2.718300e+00, double* %15, align 8
  %16 = load double, double* %14, align 8
  %17 = load double, double* %15, align 8
  %eq3 = fcmp oeq double %16, %17
  %18 = alloca i1, align 1
  store i1 %eq3, i1* %18, align 1
  %19 = load i1, i1* %18, align 1
  br i1 %19, label %then4, label %else5

then4:                                            ; preds = %else2
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @2, i32 0, i32 0))
  br label %end

else5:                                            ; preds = %else2
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @3, i32 0, i32 0))
  br label %end

end:                                              ; preds = %else5, %then4
  br label %end6

end6:                                             ; preds = %end, %then1
  br label %end7

end7:                                             ; preds = %end6, %then
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}
