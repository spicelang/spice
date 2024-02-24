; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"IsTrue: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_Z6isTruev()
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

declare i1 @_Z6isTruev()

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
