; ModuleID = 'source.spice'
source_filename = "source.spice"

$asan.module_ctor = comdat any

@__asan_option_detect_stack_use_after_return = external global i32
@___asan_gen_stack = private unnamed_addr constant [16 x i8] c"1 32 8 6 iPtr:4\00", align 1
@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@___asan_globals_registered = common hidden global i64 0
@__start_asan_globals = extern_weak hidden global i64
@__stop_asan_globals = extern_weak hidden global i64
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr @asan.module_ctor }]

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_address uwtable
define dso_local noundef i32 @main() #0 !dbg !10 {
  %result = alloca i32, align 4
  %asan_local_stack_base = alloca i64, align 8
  %1 = load i32, ptr @__asan_option_detect_stack_use_after_return, align 4
  %2 = icmp ne i32 %1, 0
  br i1 %2, label %3, label %5

3:                                                ; preds = %0
  %4 = call i64 @__asan_stack_malloc_0(i64 64)
  br label %5

5:                                                ; preds = %0, %3
  %6 = phi i64 [ 0, %0 ], [ %4, %3 ]
  %7 = inttoptr i64 %6 to ptr
  %8 = icmp eq i64 %6, 0
  br i1 %8, label %9, label %10

9:                                                ; preds = %5
  %MyAlloca = alloca i8, i64 64, align 32
  br label %10

10:                                               ; preds = %5, %9
  %11 = phi ptr [ %7, %5 ], [ %MyAlloca, %9 ]
  store ptr %11, ptr %asan_local_stack_base, align 8
  %12 = getelementptr i8, ptr %11, i64 32
  store i64 1102416563, ptr %11, align 8
  %13 = getelementptr i8, ptr %11, i64 8
  store i64 ptrtoint (ptr @___asan_gen_stack to i64), ptr %13, align 8
  %14 = getelementptr i8, ptr %11, i64 16
  store i64 ptrtoint (ptr @main to i64), ptr %14, align 8
  %15 = ptrtoint ptr %11 to i64
  %16 = lshr i64 %15, 3
  %17 = add i64 %16, 2147450880
  %18 = add i64 %17, 0
  %19 = inttoptr i64 %18 to ptr
  store i64 -868082052615769615, ptr %19, align 1
  call void @llvm.lifetime.start.p0(ptr %result), !dbg !16
    #dbg_declare(ptr %result, !17, !DIExpression(), !16)
  store i32 0, ptr %result, align 4, !dbg !16
  %20 = call ptr @_Z12sAllocUnsafem(i64 4), !dbg !18
  %21 = ptrtoint ptr %20 to i64, !dbg !18
  %22 = lshr i64 %21, 3, !dbg !18
  %23 = add i64 %22, 2147450880, !dbg !18
  %24 = inttoptr i64 %23 to ptr, !dbg !18
  %25 = load i8, ptr %24, align 1, !dbg !18
  %26 = icmp ne i8 %25, 0, !dbg !18
  br i1 %26, label %27, label %33, !dbg !18, !prof !19

27:                                               ; preds = %10
  %28 = and i64 %21, 7, !dbg !18
  %29 = add i64 %28, 3, !dbg !18
  %30 = trunc i64 %29 to i8, !dbg !18
  %31 = icmp sge i8 %30, %25, !dbg !18
  br i1 %31, label %32, label %33, !dbg !18

32:                                               ; preds = %27
  call void @__asan_report_store4(i64 %21) #4, !dbg !18
  unreachable

33:                                               ; preds = %27, %10
  store i32 0, ptr %20, align 4, !dbg !18
  %34 = add i64 %17, 4, !dbg !18
  %35 = inttoptr i64 %34 to ptr, !dbg !18
  store i8 0, ptr %35, align 1, !dbg !18
  store ptr %20, ptr %12, align 8, !dbg !18
    #dbg_declare(ptr %asan_local_stack_base, !20, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 32), !22)
  %36 = load ptr, ptr %12, align 8, !dbg !23
  %37 = ptrtoint ptr %36 to i64, !dbg !24
  %38 = lshr i64 %37, 3, !dbg !24
  %39 = add i64 %38, 2147450880, !dbg !24
  %40 = inttoptr i64 %39 to ptr, !dbg !24
  %41 = load i8, ptr %40, align 1, !dbg !24
  %42 = icmp ne i8 %41, 0, !dbg !24
  br i1 %42, label %43, label %49, !dbg !24, !prof !19

43:                                               ; preds = %33
  %44 = and i64 %37, 7, !dbg !24
  %45 = add i64 %44, 3, !dbg !24
  %46 = trunc i64 %45 to i8, !dbg !24
  %47 = icmp sge i8 %46, %41, !dbg !24
  br i1 %47, label %48, label %49, !dbg !24

48:                                               ; preds = %43
  call void @__asan_report_store4(i64 %37) #4, !dbg !24
  unreachable

49:                                               ; preds = %43, %33
  store i32 123, ptr %36, align 4, !dbg !24
  call void @_Z8sDeallocRPh(ptr noundef %12), !dbg !25
  %50 = load ptr, ptr %12, align 8, !dbg !27
  %51 = ptrtoint ptr %50 to i64, !dbg !28
  %52 = lshr i64 %51, 3, !dbg !28
  %53 = add i64 %52, 2147450880, !dbg !28
  %54 = inttoptr i64 %53 to ptr, !dbg !28
  %55 = load i8, ptr %54, align 1, !dbg !28
  %56 = icmp ne i8 %55, 0, !dbg !28
  br i1 %56, label %57, label %63, !dbg !28, !prof !19

57:                                               ; preds = %49
  %58 = and i64 %51, 7, !dbg !28
  %59 = add i64 %58, 3, !dbg !28
  %60 = trunc i64 %59 to i8, !dbg !28
  %61 = icmp sge i8 %60, %55, !dbg !28
  br i1 %61, label %62, label %63, !dbg !28

62:                                               ; preds = %57
  call void @__asan_report_store4(i64 %51) #4, !dbg !28
  unreachable

63:                                               ; preds = %57, %49
  store i32 321, ptr %50, align 4, !dbg !28
  call void @_Z8sDeallocRPh(ptr %12), !dbg !29
  %64 = add i64 %17, 4, !dbg !29
  %65 = inttoptr i64 %64 to ptr, !dbg !29
  store i8 -8, ptr %65, align 1, !dbg !29
  %66 = load i32, ptr %result, align 4, !dbg !29
  store i64 1172321806, ptr %11, align 8, !dbg !29
  %67 = icmp ne i64 %6, 0, !dbg !29
  br i1 %67, label %68, label %74, !dbg !29

68:                                               ; preds = %63
  %69 = add i64 %17, 0, !dbg !29
  %70 = inttoptr i64 %69 to ptr, !dbg !29
  store i64 -723401728380766731, ptr %70, align 1, !dbg !29
  %71 = getelementptr i8, ptr %7, i64 56, !dbg !29
  %72 = load i64, ptr %71, align 8, !dbg !29
  %73 = inttoptr i64 %72 to ptr, !dbg !29
  store i8 0, ptr %73, align 1, !dbg !29
  br label %77, !dbg !29

74:                                               ; preds = %63
  %75 = add i64 %17, 0, !dbg !29
  %76 = inttoptr i64 %75 to ptr, !dbg !29
  store i64 0, ptr %76, align 1, !dbg !29
  br label %77, !dbg !29

77:                                               ; preds = %74, %68
  ret i32 %66, !dbg !29
}

; Function Attrs: nobuiltin nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0(ptr captures(none)) #1

declare ptr @_Z12sAllocUnsafem(i64)

declare void @_Z8sDeallocRPh(ptr)

; Function Attrs: nobuiltin nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0(ptr captures(none)) #1

declare void @__asan_report_load_n(i64, i64)

declare void @__asan_loadN(i64, i64)

declare void @__asan_report_load1(i64)

declare void @__asan_load1(i64)

declare void @__asan_report_load2(i64)

declare void @__asan_load2(i64)

declare void @__asan_report_load4(i64)

declare void @__asan_load4(i64)

declare void @__asan_report_load8(i64)

declare void @__asan_load8(i64)

declare void @__asan_report_load16(i64)

declare void @__asan_load16(i64)

declare void @__asan_report_store_n(i64, i64)

declare void @__asan_storeN(i64, i64)

declare void @__asan_report_store1(i64)

declare void @__asan_store1(i64)

declare void @__asan_report_store2(i64)

declare void @__asan_store2(i64)

declare void @__asan_report_store4(i64)

declare void @__asan_store4(i64)

declare void @__asan_report_store8(i64)

declare void @__asan_store8(i64)

declare void @__asan_report_store16(i64)

declare void @__asan_store16(i64)

declare void @__asan_report_exp_load_n(i64, i64, i32)

declare void @__asan_exp_loadN(i64, i64, i32)

declare void @__asan_report_exp_load1(i64, i32)

declare void @__asan_exp_load1(i64, i32)

declare void @__asan_report_exp_load2(i64, i32)

declare void @__asan_exp_load2(i64, i32)

declare void @__asan_report_exp_load4(i64, i32)

declare void @__asan_exp_load4(i64, i32)

declare void @__asan_report_exp_load8(i64, i32)

declare void @__asan_exp_load8(i64, i32)

declare void @__asan_report_exp_load16(i64, i32)

declare void @__asan_exp_load16(i64, i32)

declare void @__asan_report_exp_store_n(i64, i64, i32)

declare void @__asan_exp_storeN(i64, i64, i32)

declare void @__asan_report_exp_store1(i64, i32)

declare void @__asan_exp_store1(i64, i32)

declare void @__asan_report_exp_store2(i64, i32)

declare void @__asan_exp_store2(i64, i32)

declare void @__asan_report_exp_store4(i64, i32)

declare void @__asan_exp_store4(i64, i32)

declare void @__asan_report_exp_store8(i64, i32)

declare void @__asan_exp_store8(i64, i32)

declare void @__asan_report_exp_store16(i64, i32)

declare void @__asan_exp_store16(i64, i32)

declare ptr @__asan_memmove(ptr, ptr, i64)

declare ptr @__asan_memcpy(ptr, ptr, i64)

declare ptr @__asan_memset(ptr, i32, i64)

declare void @__asan_handle_no_return()

declare void @__sanitizer_ptr_cmp(i64, i64)

declare void @__sanitizer_ptr_sub(i64, i64)

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.shared(ptr) #2

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.private(ptr) #2

declare i64 @__asan_stack_malloc_0(i64)

declare void @__asan_stack_free_0(i64, i64)

declare i64 @__asan_stack_malloc_1(i64)

declare void @__asan_stack_free_1(i64, i64)

declare i64 @__asan_stack_malloc_2(i64)

declare void @__asan_stack_free_2(i64, i64)

declare i64 @__asan_stack_malloc_3(i64)

declare void @__asan_stack_free_3(i64, i64)

declare i64 @__asan_stack_malloc_4(i64)

declare void @__asan_stack_free_4(i64, i64)

declare i64 @__asan_stack_malloc_5(i64)

declare void @__asan_stack_free_5(i64, i64)

declare i64 @__asan_stack_malloc_6(i64)

declare void @__asan_stack_free_6(i64, i64)

declare i64 @__asan_stack_malloc_7(i64)

declare void @__asan_stack_free_7(i64, i64)

declare i64 @__asan_stack_malloc_8(i64)

declare void @__asan_stack_free_8(i64, i64)

declare i64 @__asan_stack_malloc_9(i64)

declare void @__asan_stack_free_9(i64, i64)

declare i64 @__asan_stack_malloc_10(i64)

declare void @__asan_stack_free_10(i64, i64)

declare void @__asan_poison_stack_memory(i64, i64)

declare void @__asan_unpoison_stack_memory(i64, i64)

declare void @__asan_set_shadow_00(i64, i64)

declare void @__asan_set_shadow_01(i64, i64)

declare void @__asan_set_shadow_02(i64, i64)

declare void @__asan_set_shadow_03(i64, i64)

declare void @__asan_set_shadow_04(i64, i64)

declare void @__asan_set_shadow_05(i64, i64)

declare void @__asan_set_shadow_06(i64, i64)

declare void @__asan_set_shadow_07(i64, i64)

declare void @__asan_set_shadow_f1(i64, i64)

declare void @__asan_set_shadow_f2(i64, i64)

declare void @__asan_set_shadow_f3(i64, i64)

declare void @__asan_set_shadow_f5(i64, i64)

declare void @__asan_set_shadow_f8(i64, i64)

declare void @__asan_alloca_poison(i64, i64)

declare void @__asan_allocas_unpoison(i64, i64)

declare void @__asan_before_dynamic_init(i64)

declare void @__asan_after_dynamic_init()

declare void @__asan_register_globals(i64, i64)

declare void @__asan_unregister_globals(i64, i64)

declare void @__asan_register_image_globals(i64)

declare void @__asan_unregister_image_globals(i64)

declare void @__asan_register_elf_globals(i64, i64, i64)

declare void @__asan_unregister_elf_globals(i64, i64, i64)

declare void @__asan_init()

; Function Attrs: nounwind uwtable
define internal void @asan.module_ctor() #3 comdat {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  call void @__asan_register_elf_globals(i64 ptrtoint (ptr @___asan_globals_registered to i64), i64 ptrtoint (ptr @__start_asan_globals to i64), i64 ptrtoint (ptr @__stop_asan_globals to i64))
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_address uwtable }
attributes #1 = { nobuiltin nocallback nofree nosync nounwind willreturn }
attributes #2 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nounwind uwtable "frame-pointer"="all" }
attributes #4 = { nomerge }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.ident = !{!7}
!llvm.dbg.cu = !{!8}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{i32 4, !"nosanitize_address", i32 1}
!7 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!8 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !9, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!9 = !DIFile(filename: "/data/i516467/spice/build/test/./test-files/irgenerator/instrumentation/success-asan-and-dbg-info/source.spice", directory: "./test-files/irgenerator/instrumentation/success-asan-and-dbg-info")
!10 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !11, file: !11, line: 3, type: !12, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !8, retainedNodes: !15)
!11 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-asan-and-dbg-info")
!12 = !DISubroutineType(types: !13)
!13 = !{!14}
!14 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!15 = !{}
!16 = !DILocation(line: 3, column: 1, scope: !10)
!17 = !DILocalVariable(name: "result", scope: !10, file: !11, line: 3, type: !14)
!18 = !DILocation(line: 4, column: 22, scope: !10)
!19 = !{!"branch_weights", i32 1, i32 1048575}
!20 = !DILocalVariable(name: "iPtr", scope: !10, file: !11, line: 4, type: !21)
!21 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!22 = !DILocation(line: 4, column: 5, scope: !10)
!23 = !DILocation(line: 5, column: 6, scope: !10)
!24 = !DILocation(line: 5, column: 13, scope: !10)
!25 = !DILocation(line: 7, column: 35, scope: !26)
!26 = distinct !DILexicalBlock(scope: !10, file: !11, line: 6, column: 5)
!27 = !DILocation(line: 9, column: 6, scope: !10)
!28 = !DILocation(line: 9, column: 13, scope: !10)
!29 = !DILocation(line: 10, column: 1, scope: !10)
