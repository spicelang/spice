; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.DemoStruct = type { i32, i64 }

@printf.str.0 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [4 x i8] c"int\00", align 4
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [4 x i8] c"int\00", align 4
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [11 x i8] c"DemoStruct\00", align 4
@printf.str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [11 x i8] c"DemoStruct\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %s = alloca %struct.DemoStruct, align 8
  store i32 0, ptr %result, align 4
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef @anon.string.0)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef @anon.string.1)
  store %struct.DemoStruct { i32 123, i64 56 }, ptr %s, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr noundef @anon.string.2)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, ptr noundef @anon.string.3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
