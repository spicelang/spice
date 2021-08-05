; ModuleID = 'Module'
source_filename = "Module"

@0 = private unnamed_addr constant [8 x i8] c"Step %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
main_entry:
  %i = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for_cond

for_cond:                                         ; preds = %for, %main_entry
  %0 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %0, 10
  br i1 %lt, label %for, label %for_end

for:                                              ; preds = %for_cond
  %1 = load i32, i32* %i, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @0, i32 0, i32 0), i32 %1)
  %3 = load i32, i32* %i, align 4
  %ple = add i32 %3, 2
  store i32 %ple, i32* %i, align 4
  br label %for_cond

for_end:                                          ; preds = %for_cond
  ret i32 0
}
