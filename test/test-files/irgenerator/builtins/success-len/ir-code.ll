; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@printf.str.0 = private unnamed_addr constant [18 x i8] c"Array length: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %testIntArray = alloca [4 x i32], align 4
  store i32 0, ptr %result, align 4
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], ptr %testIntArray, align 4
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 4)
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
