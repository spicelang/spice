; ModuleID = 'Module'
source_filename = "Module"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @"faculty(int)"(i32 %0) {
entry:
  %input = alloca i32, align 4
  store i32 %0, i32* %input, align 4
  %result = alloca i32, align 4
  %1 = load i32, i32* %input, align 4
  %lt = icmp slt i32 %1, 2
  br i1 %lt, label %then, label %end

then:                                             ; preds = %entry
  ret i32 1

end:                                              ; preds = %entry
  %2 = load i32, i32* %input, align 4
  %3 = load i32, i32* %input, align 4
  %sub = sub i32 %3, 1
  %4 = call i32 @"faculty(int)"(i32 %sub)
  %mul = mul i32 %2, %4
  store i32 %mul, i32* %result, align 4
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}

define i32 @main() {
main_entry:
  %faculty = alloca i32, align 4
  %input = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1000, i32* %input, align 4
  %0 = load i32, i32* %input, align 4
  %1 = call i32 @"faculty(int)"(i32 %0)
  store i32 %1, i32* %faculty, align 4
  %2 = load i32, i32* %input, align 4
  %3 = load i32, i32* %faculty, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %2, i32 %3)
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}
