; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [12 x i8] c"F1 called.\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"F2 called.\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i1 @_Z2f1v() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i1 false
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

define private i1 @_Z2f2v() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret i1 true
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_Z2f1v()
  %2 = call i1 @_Z2f2v()
  %3 = select i1 %1, i1 %1, i1 %2
  %4 = zext i1 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
