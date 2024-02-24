; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Char: %c\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %test = alloca ptr, align 8
  %c1 = alloca i8, align 1
  store i32 0, ptr %result, align 4
  store ptr @anon.string.0, ptr %test, align 8
  %1 = load ptr, ptr %test, align 8
  %2 = getelementptr inbounds i8, ptr %1, i32 2
  %3 = load i8, ptr %2, align 1
  store i8 %3, ptr %c1, align 1
  %4 = load i8, ptr %c1, align 1
  %5 = zext i8 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
