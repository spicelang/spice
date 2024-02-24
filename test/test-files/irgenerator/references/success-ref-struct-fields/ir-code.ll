; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { i1, ptr }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Before: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [10 x i8] c"After: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca i32, align 4
  %ts = alloca %struct.TestStruct, align 8
  store i32 0, ptr %result, align 4
  store i32 123, ptr %t, align 4
  store i1 true, ptr %ts, align 1
  %1 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  store ptr %t, ptr %1, align 8
  %f2_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  %2 = load ptr, ptr %f2_addr, align 8
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %f2_addr1 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  %5 = load ptr, ptr %f2_addr1, align 8
  %6 = load i32, ptr %5, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr %5, align 4
  %8 = load i32, ptr %t, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
