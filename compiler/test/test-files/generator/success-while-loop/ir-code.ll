; ModuleID = 'Module'
source_filename = "Module"

@0 = private unnamed_addr constant [16 x i8] c"i is now at: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
main_entry:
  %i = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %i, align 4
  %0 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %0, 10
  br i1 %lt, label %while, label %while_end

while:                                            ; preds = %while, %main_entry
  %1 = load i32, i32* %i, align 4
  %ple = add i32 %1, 1
  store i32 %ple, i32* %i, align 4
  %2 = load i32, i32* %i, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0), i32 %2)
  %4 = load i32, i32* %i, align 4
  %lt1 = icmp slt i32 %4, 10
  br i1 %lt1, label %while, label %while_end

while_end:                                        ; preds = %while, %main_entry
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}
