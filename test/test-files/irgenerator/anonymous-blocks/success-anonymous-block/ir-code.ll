; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [14 x i8] c"Test var: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %testVar = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 12, ptr %testVar, align 4
  %1 = load i32, ptr %testVar, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
