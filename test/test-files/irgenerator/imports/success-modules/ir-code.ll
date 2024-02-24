; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %integer = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_Z20forwardToOtherModulev()
  store i32 %1, ptr %integer, align 4
  %2 = load i32, ptr %integer, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare i32 @_Z20forwardToOtherModulev()

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
