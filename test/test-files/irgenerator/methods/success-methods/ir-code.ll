; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Letter = type { ptr }

@anon.string.0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.0 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1

define private ptr @_ZN6Letter10getContentEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %content_addr = getelementptr inbounds %struct.Letter, ptr %2, i64 0, i32 0
  %3 = load ptr, ptr %content_addr, align 8
  ret ptr %3
}

define private void @_ZN6Letter10setContentEPKc(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr %1) {
  %this = alloca ptr, align 8
  %text = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %text, align 8
  %3 = load ptr, ptr %this, align 8
  %content_addr = getelementptr inbounds %struct.Letter, ptr %3, i64 0, i32 0
  %4 = load ptr, ptr %text, align 8
  store ptr %4, ptr %content_addr, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %letter = alloca %struct.Letter, align 8
  store i32 0, ptr %result, align 4
  store %struct.Letter { ptr @anon.string.0 }, ptr %letter, align 8
  call void @_ZN6Letter10setContentEPKc(ptr noundef nonnull align 8 dereferenceable(8) %letter, ptr @anon.string.1)
  %1 = call ptr @_ZN6Letter10getContentEv(ptr noundef nonnull align 8 dereferenceable(8) %letter)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
