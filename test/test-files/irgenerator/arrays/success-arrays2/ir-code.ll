; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.array.0 = private unnamed_addr constant [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512]
@printf.str.0 = private unnamed_addr constant [17 x i8] c"intArray[3]: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"intArray[7]: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"intArray[9]: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %intArray = alloca [10 x i32], align 4
  store i32 0, ptr %result, align 4
  store [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512], ptr %intArray, align 4
  %1 = getelementptr inbounds [10 x i32], ptr %intArray, i64 0, i32 3
  %2 = load i32, ptr %1, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = getelementptr inbounds [10 x i32], ptr %intArray, i64 0, i32 7
  %5 = load i32, ptr %4, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5)
  %7 = getelementptr inbounds [10 x i32], ptr %intArray, i64 0, i32 9
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
